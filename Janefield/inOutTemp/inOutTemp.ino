// 18 / 01 /18
// nodeMCU thingspeak client with IR send/receive support and webserver for Temp /Humidity checking live on the fly. from anywhere.
// digital 2 / gpio 4 --- temp DHT11
// dig 1 / gp 5 -- ir rec @ (d1)---gpio 5 ?
// d6 / gp12  -- ir sender

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <RestClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ThingSpeak.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

// COMMENT OUT DEPENDING ON WHICH ONE....
//////////////////////
// inside
 //IPAddress ip(192, 168, 78, 188); ESP8266WebServer server(9188);  int Tfield = 1;   int Hfield = 2;const char* host = "nodeinside";
// Outside
IPAddress ip(192, 168, 78, 189); ESP8266WebServer server(9189); int Tfield = 3;  int Hfield = 4;const char* host = "nodeoutside";


float prevTemp;
long t = 0;

int rgb_pins[] = {14, 13, 15};
#define Red 14     // d5...gp 14
#define Green 13    // D7.. gp 13
#define Blue 15     // .D8 .. gp 15
#define RECV_PIN 5  /// d1 ... 
#define SEND_PIN 12  /// d6?
#define DHTPIN     4   // d2
//////////////////////

bool held = false;
IRsend irsend(SEND_PIN);
unsigned long lightdone = 0;
#define LED     16        // Led in NodeMCU at pin GPIO16 (D0).
#define BRIGHT    350     //max led intensity (1-500)
#define INHALE    1250    //Inhalation time in milliseconds.
#define PULSE     INHALE*1000/BRIGHT
#define REST      1000    //Rest Between Inhalations.

// TEMPC AND HUMIDITY
#define DHTTYPE           DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
unsigned long myChannelNumber = 404585;
const char * myWriteAPIKey = "W124WS7UN76VCASZ";
int value = 0;
char temperatureString[6]; char humidString[6]; float temp, humid;

// IR
IRrecv irrecv(RECV_PIN); decode_results results;

// HUE LIGHTING
const char LIGHTS_ON[] = "{\"on\":true}";
const char LIGHTS_OFF[] = "{\"on\":false}";
const char *bridge_ip = "192.168.0.101"; // Hue Bridge IP
bool looping;

// Wi-Fi / IoT
const char ssid[] = "TPG 15EA";
const char password[] = "abcd1989";
const int port = 80;   // fix IP of this node
IPAddress gateway(192, 168, 78, 78);  // WiFi router's IP
IPAddress subnet(255, 255, 255, 0);
RestClient hue = RestClient(bridge_ip);
WiFiClient client;
unsigned long mLastTime1, mLastTime, holdup = 0;

void setColor(long red, long green, long blue, int wai) {
  held = true;
  holdup = ((wai * 1000) + millis());
  analogWrite(Red, red);
  analogWrite(Green, green);
  analogWrite(Blue, blue);
}
void lightLoop() {
  for (int i = 1; i < BRIGHT; i++) {
    digitalWrite(LED, LOW);          // turn the LED on.
    delayMicroseconds(i * 10);       // wait
    digitalWrite(LED, HIGH);         // turn the LED off.
    delayMicroseconds(PULSE - i * 10); // wait
    delay(0);                        //to prevent watchdog firing.
  }
  //ramp decreasing intensity, Exhalation (half time):
  for (int i = BRIGHT - 1; i > 0; i--) {
    digitalWrite(LED, LOW);          // turn the LED on.
    delayMicroseconds(i * 10);        // wait
    digitalWrite(LED, HIGH);         // turn the LED off.
    delayMicroseconds(PULSE - i * 10); // wait
    i--;
    delay(0);                        //to prevent watchdog firing.
  }
  lightdone = millis();
}

void getTemperature() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    temp = event.temperature;
    Serial.println(" *C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    humid = event.relative_humidity;
    Serial.println("%");
  }
  dtostrf(temp, 2, 2 , temperatureString);
  dtostrf(humid, 2, 2 , humidString);
  ThingSpeak.setField(Tfield, temp);
  ThingSpeak.setField(Hfield, humid);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  lightLoop();
  
}

// web comms
void handleRoot() {
  String message = "<html><head><title>NodeIR TPad</title></head><body><h1>Temp Outside</h1>";
  message += "<h2>Temp =";
  message += temperatureString;
  message += "</h2>";
  message += "<h2>Humid =";
  message += humidString;
  message += "</h2><br><h2>LoungeCommands</h2><p><a href=\"tvoff\">Send TV On/Off</a></p><p><a href=\"volup\">Send VolUP</a></p><p><a href=\"voldn\">Send VolDN</a></p><p><a href=\"tvaux\">HiFi TV AUX</a></p><p><a href=\"btaux\">HiFi BT AUX</a></p></body></html>";
  server.send(200, "text/html", message);
}
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  server.send(404, "text/plain", message);
  Serial.print(message);
}

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet);
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);
}


/// IR COMMANDS

/**
  void volup() {
  sendIr('HiFi_Vup');
  }
  void voldn() {
  sendIr(HiFi_Vdn);
  }
  void tvaux() {
  sendIr(HiFi_TV);
  }
  void tvoff(){
  sendIr(TV_OFF);
  }
  void btaux(){
  sendIr(HiFi_BT);
  }
**/

void sendIr(uint64_t cmd) {
  if (cmd == 'TV_OFF') {
#define cmdw 0xa90
  }
  else {
#define cmdw 0xa90
  }
  irsend.begin();
  Serial.println("sending Sony");
  irsend.sendSony(cmdw, 12, 2);
  delay(200);
  irrecv.resume();
}
void dump(decode_results *results) {
  uint16_t count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  } else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");
  } else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  } else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  } else if (results->decode_type == RC5X) {
    Serial.print("Decoded RC5X: ");
  } else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  } else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  } else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  } else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  } else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  } else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  }
  serialPrintUint64(results->value, 16);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");
  for (uint16_t i = 1; i < count; i++) {
    if (i % 100 == 0)
      yield();  // Preemptive yield every 100th entry to feed the WDT.
    if (i & 1) {
      //         Serial.print(results.rawbuf[i] * USECPERTICK, DEC);
    } else {
      Serial.write('-');
      //      Serial.print((uint32_t) results.rawbuf[i] * USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println();
}

uint32_t delayMS;

/// HUE COMMANDS
//
void doLoop(int on, int lightNo) {
  if (on == 1) {
    const char EFFECT_COLORLOOP[] = "{\"effect\":\"colorloop\"}";
    String cmd = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
    cmd += lightNo;   cmd += "/state/";
    //  hue.put(cmd, EFFECT_COLORLOOP);
    looping = true;
  }
  if (on == 0) {
    const char NO_COLORLOOP[] = "{\"effect\":\"none\"}";
    String cmd = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
    cmd += lightNo;   cmd += "/state/";
    //hue.put(cmd, NO_COLORLOOP);
    looping = false;
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
for (int i=0; i<3; i++)
  {
    pinMode(rgb_pins[i], OUTPUT);
   digitalWrite(rgb_pins[i],LOW);
  }
  setup_wifi();
    ArduinoOTA.setHostname(host);
    // turn of leds 
    digitalWrite(Red,HIGH);
    digitalWrite(Green,HIGH);
   digitalWrite(Blue,HIGH);
  /* configure dimmers, and OTA server events */
  analogWriteRange(1000);
  analogWrite(Blue,990);
  
  dht.begin();
  sensor_t sensor; dht.temperature().getSensor(&sensor); Serial.println("------------------------------------"); Serial.println("Temperature");  Serial.print  ("Sensor:       "); Serial.println(sensor.name);  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version); Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C"); Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C"); Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C"); Serial.println("------------------------------------");
  dht.humidity().getSensor(&sensor); Serial.println("------------------------------------");  Serial.println("Humidity");  Serial.print  ("Sensor:       "); Serial.println(sensor.name);  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id); Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  Serial.println("------------------------------------");
  delayMS = sensor.min_delay / 1000;
  ThingSpeak.begin(client);

  //server.on("/", handleRoot); //server.on("/volup", volup); server.on("/btaux", btaux); server.on("/tvaux", tvaux); server.on("/voldn", voldn); server.on("/tvoff", [](){ sendIr(TV_OFF); server.send(200, "text/plain", "tv OFF"); });
  //server.onNotFound(handleNotFound); server.begin();
  delay(100);
  irrecv.enableIRIn();
  Serial.println("setup complete.");
  
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
                        for(int i=0; i<3;i++)
                          analogWrite(rgb_pins[i], 0);
                                      });
  ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
                          for (int i=0;i<30;i++)
                          {
                            analogWrite(Blue,(i*100) % 1001);
                            delay(50);
                             analogWrite(Green,(1-(i*100) % 1001));
                          }
                        });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  ArduinoOTA.handle();
  unsigned long time = millis();
  //if (held == true) {
   // if (holdup > 0) {
//holdup=holdup-time;
  //  } else {
    //setColor(0,0,1000,2);
   // }
 // }
if (irrecv.decode(&results)) {
  dump(&results);
  irrecv.resume();
}
//server.handleClient();
if ((time - mLastTime1) >= 30000) {
  mLastTime1 = millis();
  getTemperature();
if (temp != prevTemp){
  if (temp > prevTemp){
analogWrite(Red, 1000);
  } else if (temp < prevTemp){
analogWrite(Green, 1000);
}
  prevTemp = temp;
}
}
delay(10);
}

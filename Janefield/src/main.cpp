#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <ESP8266WiFi.h>
//#include <RestClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <dht11.h>
#include <ThingSpeak.h>

//D0 = GPIO16, D1=GP5,D2=GP4,D4=GP2,D5=GP14,D6=GP12,D7=GP13,D8=GP15,SD3=GP10,SD2 =GP9
// THINGSPEAK
unsigned long myChannelNumber = 404585;
const char * myWriteAPIKey = "W124WS7UN76VCASZ";

// diff units
// COMMENT OUT DEPENDING ON WHICH ONE....
// inside
// ESP8266WebServer server(9188);  IPAddress ip(192, 168, 78, 188);    int Tfield = 1;   int Hfield = 2;   int dhPin = 2;gp

// Outside
 int dhPin = 9;  IPAddress ip(192, 168, 78, 189); ESP8266WebServer server(9189); int Tfield = 3;  int Hfield = 4;
//////////////////////

dht11 DHT11;
#define DHT11PIN dhPin

//byte ledPin = 2;
// TEMPC AND HUMIDITY
int value = 0;

char temperatureString[6];
char humidString[6];
float temp, humid;

// IR
uint16_t RECV_PIN = 0;   // IR detector @ GPIO pin 5 (d1)
IRsend irsend(4);  // d6  ......  gpio 5=pin d1
IRrecv irrecv(RECV_PIN);
decode_results results;

//  comands
#define TV_OFF        0xa90
#define HiFi_OFF        0x540a
#define HiFi_TV        0x540a
#define HiFi_BT        0x540a
#define HiFi_Vup        0x240a
#define HiFi_Vdn        0x640a
#define L1_ON        0x640a
#define L1_OFF    	0x640a
#define L2_ON        0x640a
#define L2_OFF        0x640a
// HUE LIGHTING
const char LIGHTS_ON[] = "{\"on\":true}";
const char LIGHTS_OFF[] = "{\"on\":false}";
const char *bridge_ip = "192.168.0.101"; // Hue Bridge IP
bool looping;

// Wi-Fi / IoT
const char ssid[] = "TPG 15EA";
const char password[] = "abcd1989";
const int port = 80;   // fix IP of this node
IPAddress gateway(192,168,78,78);     // WiFi router's IP
IPAddress subnet(255,255,255,0);
//  RestClient hue = RestClient(bridge_ip);
WiFiClient client;
unsigned long mLastTime1, mLastTime=0;


void doLoop(int on, int lightNo){
  if (on == 1){
	const char EFFECT_COLORLOOP[] = "{\"effect\":\"colorloop\"}";
		String cmd = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
		cmd += lightNo;		cmd += "/state/";
	//	hue.put(cmd, EFFECT_COLORLOOP);
		looping = true;
	}
	if (on == 0){
	const char NO_COLORLOOP[] = "{\"effect\":\"none\"}";
		String cmd = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
		cmd += lightNo;		cmd += "/state/";
		//hue.put(cmd, NO_COLORLOOP);
		looping = false;
	}
}

void sendStats(){
  ThingSpeak.setField(Tfield,temp);ThingSpeak.setField(Hfield,humid);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  delay(500);
  //digitalWrite(ledPin, LOW);
}

void getTemperature(){
  //  digitalWrite(ledPin, HIGH);
int chk = DHT11.read(DHT11PIN);
    Serial.print("Read sensor: ");
    switch (chk)
    {
      case DHTLIB_OK:
  		Serial.println("OK");
  		break;
      case DHTLIB_ERROR_CHECKSUM:
  		Serial.println("Checksum error");
  		break;
      case DHTLIB_ERROR_TIMEOUT:
  		Serial.println("Time out error");
  		break;
      default:
  		Serial.println("Unknown error");
  		break;
    }
  humid = (float)DHT11.humidity;
  temp =  (float)DHT11.temperature;
    Serial.print("Humidity (%): ");
    Serial.println(humid);
    Serial.print("Temperature (°C): ");
    Serial.println(temp);

  dtostrf(temp,2,2 , temperatureString);
  dtostrf(humid,2,2 , humidString);
  sendStats();
}

void sendIr(uint64_t cmd){
  irsend.begin();
  Serial.println("sending Sony");
  irsend.sendSony(cmd, 12, 2);
  delay(100);
}

void handleRoot() {
  String message = "<html><head><title>NodeIR TPad</title></head><body><h1>Temp Outside</h1>";
  message += "<h2>Temp =";
  message += temperatureString;
  message += "</h2>";
  message += "<h2>Humid =";
  message += humidString;
  message +=  "</h2>";
  message += "</h2><br><h2>LoungeCommands</h2><p><a href=\"tvoff\">Send TV On/Off</a></p><p><a href=\"volup\">Send VolUP</a></p><p><a href=\"voldn\">Send VolDN</a></p><p><a href=\"tvaux\">HiFi TV AUX</a></p><p><a href=\"btaux\">HiFi BT AUX</a></p></body></html>";
  server.send(200, "text/html", message);
}

void volup() {
  sendIr(HiFi_Vup);
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

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  server.send(404, "text/plain", message);
  Serial.print(message);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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

void setup() {
  Serial.begin(115200);
  setup_wifi();
//  pinMode(ledPin, OUTPUT);
  ThingSpeak.begin(client);
  server.on("/", handleRoot);
  server.on("/volup", volup);
  server.on("/btaux", btaux);
  server.on("/tvaux", tvaux);
  server.on("/voldn", voldn);
  server.on("/tvoff", [](){
  sendIr(TV_OFF);
  server.send(200, "text/plain", "tv OFF");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  delay(100);
  irrecv.enableIRIn();  // Start the receiver
}

void loop() {
server.handleClient();
  if (irrecv.decode(&results)) {
    dump(&results);
    irrecv.resume();
    delay(100);
  }
  if ((millis() - mLastTime) >= 1000) {
    mLastTime = millis();
  //   digitalWrite(ledPin, LOW);
     // sendIr(HiFi_Vup);
  }
  if ((millis() - mLastTime1) >= 30000) {
  //   digitalWrite(ledPin, HIGH);
     mLastTime1 = millis();
     getTemperature();
    }
}

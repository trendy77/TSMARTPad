/*
stripped down RoomBot
reassembled - optional internet (to speed-up startup) and LCD (general ease of testing)...
5/09
*/

#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <aREST.h>
#include <CC3000_MDNS.h>
#include <avr/wdt.h>
#include <IRLib.h>
#include <Wire.h>
#include <BH1750.h>
#include <stdlib.h>
#include <OneWire.h>
#include <Average.h>
#include <LiquidTWI.h>

    int minat = 0;
    int maxat = 0;
    int minat2= 0;
    int maxat2 = 0;
<<<<<<< HEAD
IRrecv My_Receiver(A15);
#define trigPin 33    
#define echoPin 31		
#define trigPin2 37     
#define echoPin2 35
BH1750 lightMeter;
const int potDialPin = A0;
=======
IRrecv My_Receiver(A0);
#define trigPin2 33    
#define echoPin2 31		
#define trigPin 37     
#define echoPin 35
BH1750 lightMeter;
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
OneWire  ds(A3);
#define I2C_ADDR  0x20
const int piezoPin = 0;
LiquidTWI lcd(0);
int bobPos = 0;
int winPos = 0;

#define MY_PROTOCOL 	SONY
#define MY_PROTOCOL2 	NEC
//NEC CODES FOR DEBUG
#define UP_ARROW2      	0x61D6D827 //	INCREASE TILT TIME +1SEC
#define DOWN_ARROW2    	0x61D658A7 //DECREASE TILT TIME -1SEC
#define RIGHT_ARROW2   	0x61D6609F //Move All tilt FWD
#define LEFT_ARROW2    	0x61D620DF //Move All tilt BKD
#define SELECT_BUTTON2 	0x61D6A05F //	BUZZER!!

#define UP_ARROW      	0x1E108 //	INCREASE TILT TIME +1SEC
#define DOWN_ARROW    	0x9E108 //DECREASE TILT TIME -1SEC
#define RIGHT_ARROW   	0xDE108 //Move All tilt FWD
#define LEFT_ARROW    	0x5E108 //Move All tilt BKD
#define SELECT_BUTTON 	0x3E108 //	BUZZER!!
#define BUTTON_0 		0x90108  			//
#define BUTTON_1		0x108 			 // KITCHENT F
#define BUTTON_2 		0x80108		 // KITCHENT B
#define BUTTON_3 		0x40108		 // WINT F
#define BUTTON_4 		0xC0108		// WINT B
#define BUTTON_5 		0x20108		// DWT F
#define BUTTON_6 		0xa0108 		// DWT B
#define BUTTON_7		0x60108		 // LT F 
#define BUTTON_8 		0xe0108 		// LT B
#define BUTTON_9 		0x10108		 // 
#define PRESET_PREV 	0xC108 		//  LOWER LOUNGE BLIND
#define PRESET_NEXT 	0x8C108		//  RAISE LOUNGE BLIND  
#define RED				0xA010C	// close kitchen window
#define GREEN 			0x6010C	// open kitchen windwo
#define YELLOW 			0xE010C	// Close loungewin
#define BLUE 			0x2010c		// // Open loungewindow
#define SOUND_PREV 		0x7B0B			// LOWER BOB
#define SOUND_NEXT 		0x3B0B 	 // RAISE BOB
#define TOP_MENU 		0x98108
#define POPUP_MENU 		0x58108	
#define PAUSE 			0x400B	
#define STOP 			0xB
#define PLAY 			0x200B		
#define FAST_FWD		0x20C108
#define FAST_RWD		0xCC108
#define SUBTITLE		0x88108

unsigned long interval = 300000;           // every 5 minutes
unsigned long time; long nextup; long lastup;
	int tilttimer = 2000; 	//default length of "tilt fwd or bwd'
	int tiltmax = 10000;      
	int tiltmin = 1000;
	int testtimer = 1000;
	int tilttimerS = (tilttimer/1000);
	int wintimer = (tilttimer*5);
	int wintimerS = (wintimer/1000);

IRTYPES codeType;        
unsigned long codeValue; 
int codeBits; unsigned int rawCodes[RAWBUF]; 
int rawCount;                  
bool GotOne, GotNew; IRdecode My_Decoder; 
#define MY_PROTOCOL2 	NEC
#define UP_ARROW2      	0x61D6D827 //	INCREASE TILT TIME +1SEC
#define DOWN_ARROW2    	0x61D658A7 //DECREASE TILT TIME -1SEC
#define RIGHT_ARROW2   	0x61D6609F //Move All tilt FWD
#define LEFT_ARROW2    	0x61D620DF //Move All tilt BKD
#define SELECT_BUTTON2 	0x61D6A05F //	BUZZER!!

int rm_light; uint16_t lux; int lightPercent;
int rm_temp; String temperature; float celsius;float temp_c;
int lightMin =0;int lightMax = 1023;
int potDialVal = 0; 
static char tempbuffer[10];int prevPot = 0;
Average<float> aveRT(10);Average<float> aveRL(10);int logged = 0; long nonSense = 0; long nonSense2 = 0;
Average<float> aveSense1(10); Average<float> aveSense2(10);
long loopcounter;

#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
#define WEBSITE      "io.adafruit.com"
#define WEBPAGE      "/api/groups/envirosensor-feeds/send.json?x-aio-key="

uint32_t ip;
const int potDialPin = A14;
int port = 1883;
int maximumRange = 200; int minimumRange = 0; // Minimum range needed
int duration; int distance; // Duration used to calculate distance
int duration2; int distance2; // Duration used to calculate distance

uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t retarrow[8] = {	0x1,0x1,0x5,0x9,0x1f,0x8,0x4};

//int barlevel; const byte barpins[10] = {23,25,27,29,31,33,35,37,39,41};
const int ledCount = 10;

void setup(){
	Serial.begin(115200);
	Serial.println("ROOMBOT INITIALISING...");
	lcd.begin(20, 4);	lcd.setBacklight(HIGH);lcd.setCursor(0, 2);
	lcd.print("** INITIALISING.. **");

	Serial.println("LOADING SENSORS");
<<<<<<< HEAD
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	pinMode(trigPin2, OUTPUT);
	pinMode(echoPin2, INPUT);	
=======
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
	pinMode(piezoPin, OUTPUT);pinMode(potDialPin, INPUT);
	GotOne = false; GotNew = false; codeType = UNKNOWN;   codeValue = 0;	
  Wire.begin(); // Wake up I2C bus
  Wire.beginTransmission(I2C_ADDR);// Set I/O bank A to outputs
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // Set all of bank A to outputs
  Wire.endTransmission();
  delay(15);
	lightMeter.begin();
	delay(50);
<<<<<<< HEAD
  /* //TO ENABLE INTERNET :
    wdt_enable(WDTO_4S);wdt_reset();	
=======
  /*TO ENABLE INTERNET :
 //   wdt_enable(WDTO_4S);wdt_reset();	
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
		rest.variable("room_temp",&rm_temp);	rest.variable("room_light",&rm_light);      
        rest.function("raiseBob",raiseBOB);	rest.function("lowerBob",lowerBOB);	
	rest.function("buzz",buzz);	  rest.set_id("172");			rest.set_name("RoomBot");

	Serial.println("LOADING WIFI CONNECTION");
  if (!cc3000.begin())  {    while(1);  }  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {    while(1);  }  while (!cc3000.checkDHCP())  {    delay(100);  }  
  if (!mdns.begin("arduino", cc3000)) {   while(1);   }  restServer.begin();
  Serial.println("Online and listening for connections...");  
//////
  Serial.println(F("Adafruit IO Example:"));
  Serial.print(F("Free RAM: ")); Serial.println(getFreeRam(), DEC);
  // Initialise the CC3000 module
  Serial.print(F("\nInit the CC3000..."));
  if (! cc3000.begin())
    halt("Failed");
  // attempt wifi connection
  while (! CC3000connect(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Retrying WiFi"));
    while(1);}
  Serial.println(F("Connected to WiFi!"));
  // listen for events 
  mqtt.subscribe(&boblevel);
  // connect to adafruit io
  connect();

for lcd:
<<<<<<< HEAD
	lcd.begin(20, 4);	lcd.setBacklight(HIGH);lcd.setCursor(0, 2);
	lcd.print("** INITIALISING.. **");
*/
Serial.println("ROOMBOT SETUP COMPLETE");
	My_Receiver.enableIRIn(); // Start the receiver
	Serial.println("Press ZERO for Options"); 
=======
*/  
Serial.println("ROOMBOT SETUP COMPLETE");
	My_Receiver.enableIRIn(); // Start the receiver
	Serial.println("Press ZERO for Options"); 

lcd.clear();
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
}

void loop() {
time = millis();
	nextup = ((interval + lastup) - time);

/*
  Adafruit_MQTT_Subscribe *subscription;
  // Make sure to reset watchdog every loop iteration!
  Watchdog.reset();
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();  }
  // this is our 'wait for incoming subscription packets' busy subloop
  while (subscription = mqtt.readSubscription(1000)) {

    // we only care about the lamp events
    if (subscription == &boblevel) {

      // convert mqtt ascii payload to int
      char *value = (char *)boblevel.lastread;
      Serial.print(F("Received: "));
      Serial.println(value);
      int current = atoi(value);
if (current == 1){
  Watchdog.disable();autoraiseBob();
}
 }
}
*/

   windowSense();
   windowSense2();
<<<<<<< HEAD
	nextup = ((interval + lastup) - time);
=======
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
	readSensors();
updateLcd();
if (My_Receiver.GetResults(&My_Decoder)) {
   IRDetected();
	delay(100);
My_Receiver.resume();
	}

if (time > (lastup + interval)){
	Serial.println("Time to send server");
//send2server();
    logged =0;
	lastup = time;
    }

serialcomms();
loopcounter++;
if (loopcounter == 5){
Serial.print("*");
loopcounter =0;
}
}

void serialcomms(){	
	if (Serial.available()){
	int command = Serial.read();
		switch (command)
		{
		case '0':
<<<<<<< HEAD
			Serial.print("BOB@ ");
			Serial.print(distance);
			Serial.print("cm **");
			Serial.print("BOB2@ ");
			Serial.print(distance2);
			Serial.println("cm");
			Serial.println("TSMARTPad- ROOM OPTIONS");
			Serial.println(" Current Sensor Readings: "); readAndPrint();
			Serial.println(" 1/q- Auto raise/lower BOB ");
			Serial.println(" 2/w-  shortBobU/D  ");
			Serial.println(" 3/e- bob norm 4/r-motor2secs 5/t-Sense Open/CloseWin   p-pot mode ");
			Serial.println(" 9- SEND2SERVER");
=======
			Serial.println("TSMARTPad- ROOM OPTIONS");
			Serial.println(" Current Sensor Readings: "); readAndPrint();
			Serial.println(" 1/q- Auto raise/lower BOB ");
			Serial.println(" 2/w-  shortBobU/D   3/e- bob raise/lower ");
			Serial.println("4/r-window motor2secs  ^^ 'g'- WinhalfOpen");
Serial.println("	5/t-SenseOpen/CloseWin    ");
			Serial.println(" 'b' - raise both!");
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
			Serial.println(" z- reset all relays ");
			delay(1000);			break;

		case '1':    autoraiseBob();	break;
		case 'q': autolowerBob(); break;
		case '2':	shortBobU();	break;
		case 'w':shortBobD(); break;
		case '3': raiseBob(); break;
		case 'e': lowerBob(); break;
		case '4': openWin(); break;
		case 'r': closeWin(); break;
		case 'f': readAndPrint(); break;
		case '5': openSenseWin(); break;
<<<<<<< HEAD
=======
case 'g': winHalf(); break;
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
		case 't': closeSenseWin(); break;
		case '6': sendValueToLatch(32);      Serial.println("Activating relay 6"); break;
			//	sendValueToLatch(16);  Serial.println("Activating relay 5");   break;
		case '7':  sendValueToLatch(64);  Serial.println("Activating relay 7"); break;
		case '8':  sendValueToLatch(128);      Serial.println("Activating relay 8");   break;
<<<<<<< HEAD
		case 'p': 	potDialVal = analogRead(potDialPin);
			Serial.print("pot is");		Serial.println(potDialVal);
=======
		case 'b': raiseBoth(); break;
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
/*
			if (tempPot1 = !potDialVal){
				while (tempPot1 < ((potDialVal - 2) && distance>1)){
					sendValueToLatch(2);
					delay(tempPot1 * 10);
				}
				while (tempPot1 > (potDialVal + 2)){
					sendValueToLatch(1);
					delay(tempPot1 * 15);
				}
				sendValueToLatch(0);
				prevPot = potDialVal;
			}	*/
<<<<<<< HEAD
			break;
		case 'z':sendValueToLatch(0);      Serial.println("Resetting all relays");  break;
		case '9':          break;
=======

		case 'z':sendValueToLatch(0);      Serial.println("Resetting all relays");  break;
		}
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
		}
	}
    
<<<<<<< HEAD
	void openSenseWin(){
	windowSense();
if (distance < 70) {
	sendValueToLatch(32);
	while (distance < 30){
  	windowSense();
        Serial.print("dist @"); Serial.println(distance);
      }
      sendValueToLatch(0);
	delay(1000);
	}
}
void closeSenseWin(){
	windowSense();
if (distance > 10) {
	sendValueToLatch(4);
while (distance > 5){
  	windowSense();
        Serial.print("dist @"); Serial.println(distance);
      }
      sendValueToLatch(0);
	delay(1000);
}
}
	
	void IRDetected(){
=======
    
void openSenseWin(){
	windowSense2();
Serial.println("OPENING WINDOW TO 23CM...");
	sendValueToLatch(32); 
while 	(distance2 <23){
                windowSense2();
		Serial.print("dist @");Serial.println(distance2);
        delay(100);
	}
	sendValueToLatch(0); 
	delay(50);
	}
void closeSenseWin(){
  	windowSense2();
	if (distance2 >3 ) { 
	sendValueToLatch(\4); 
while 	(distance2 >3){
                windowSense2();
		Serial.print("dist @");Serial.println(distance2);
        delay(100);
	}
	sendValueToLatch(0); 
	delay(50);
	}
}	
	
void winHalf(){
windowSense2();
Serial.println("OPENING WINDOW TO halfway15CM...");
if (distance2 <15){
  	sendValueToLatch(32); 
  while (distance2 <15){
windowSense2();
Serial.print("win @");Serial.println(distance2);
  }
  sendValueToLatch(0); 
}
else if (distance2 >15){
  	sendValueToLatch(4); 
  while (distance2 <15){
windowSense2();
Serial.print("win @");Serial.println(distance2);
  }
  sendValueToLatch(0); 
}

void IRDetected(){
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
 	 Serial.println("IR COMMAND DETECTED - DECODING...");
	My_Decoder.decode(); GotOne=true;
	GotNew=true;  codeType = My_Decoder.decode_type;  if (codeType == UNKNOWN) {    
	Serial.println("Received unknown code");
    rawCount = My_Decoder.rawlen-1;    }   
	else{	Serial.print(F("Received "));    Serial.print(Pnames(codeType));
	if (My_Decoder.value == REPEAT) {	Serial.println(F("repeat; ignoring."));     } 
	 else{ codeValue = My_Decoder.value; codeBits = My_Decoder.bits;
} 	Serial.print(F(" Value:0x"));Serial.println(My_Decoder.value, HEX);
if((My_Decoder.decode_type==MY_PROTOCOL)  ||(My_Decoder.decode_type == MY_PROTOCOL2)){
Serial.println("Code is for TSMARTPad"); 
switch(My_Decoder.value) {
case UP_ARROW:     autoraiseBob(); break; 	 // lower room bob
case DOWN_ARROW:    	autolowerBob(); break;	// raise room bob
case SELECT_BUTTON: 	buzzUP();	break;
case SOUND_PREV:	shortBobD();break;
case SOUND_NEXT: shortBobU();break;
//case SUBTITLE: 
<<<<<<< HEAD
case BUTTON_0: senseMoveBob(); break;
case BUTTON_9: sendValueToLatch(0); Serial.println("resetting all relays"); break;
			}
}
else if (My_Decoder.decode_type == MY_PROTOCOL2) {
      //	leds.setLEDs(LED_YELLOW); leds.update();
      Serial.println("Code is IR command for TSMARTPad");
            switch (My_Decoder.value) {
      case DOWN_ARROW2:        printSensors();        break;
      case UP_ARROW2:printSensors();        break;
      case LEFT_ARROW2:printSensors();        break;
      case RIGHT_ARROW2:printSensors();break;
      }
    	}
=======
case BUTTON_0: Serial.println("closng window"); closeSenseWin();break;
case BUTTON_9: Serial.println("opening window");openSenseWin();  break;
case BUTTON_8: Serial.println("open half window");winHalf();  break;
     case DOWN_ARROW2:        printSensors();break;
      case UP_ARROW2:printSensors();        break;
      case LEFT_ARROW2:printSensors();        break;
      case RIGHT_ARROW2:printSensors();break;
      }		
	  }
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
	}	
}

// connect to adafruit io via MQTT
void connect() {
  Serial.print("Connecting to Adafruit IO... ");
/*  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default:
        Serial.println(F("Connection failed"));
        CC3000connect(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
        break;
    }
    if(ret >= 0)
      mqtt.disconnect();
    Serial.println(F("Retrying connection..."));
    delay(5000);
  }
  */Serial.println(F("Adafruit IO Connected!"));
}
    
void printSensors(){
	Serial.print("BOB2 DISTANCE @ ");
	Serial.print(distance2);	Serial.println("cm");
	Serial.print("BOB DISTANCE @ ");
	Serial.print(distance);	Serial.println("cm");
	Serial.print("Temperature: ");	Serial.print(temp_c);	Serial.println("C ");
	Serial.print("Light=");  Serial.print(rm_light);  Serial.print("(lux)/");  
	Serial.print(" Averages ~ ");Serial.print(logged);	Serial.println("Readings");
	Serial.print("LUX Mean:   "); Serial.println(aveRL.mean());
	Serial.print(" LUX Mode:   "); Serial.println(aveRL.mode());
	//   Serial.print("Max:    "); Serial.println(aveRL.maximum(&maxat));
	//    Serial.print(" at:    "); Serial.println(maxat);
	//   Serial.print("Min:    "); Serial.println(aveRL.minimum(&minat));
	//  Serial.print(" at:    "); Serial.println(minat);
	Serial.print("StdDev: "); Serial.println(aveRL.stddev());
	Serial.print("TEMP Mean:   "); Serial.println(aveRT.mean());
	Serial.print("TEMP Mode:   "); Serial.println(aveRT.mode());
	//   Serial.print("Max:    "); Serial.println(aveRT.maximum(&maxat));
	//    Serial.print(" at:    "); Serial.println(maxat);
	//   Serial.print("Min:    "); Serial.println(aveRT.minimum(&minat));
	//  Serial.print(" at:    "); Serial.println(minat);
	Serial.print("StdDev: "); Serial.println(aveRT.stddev());
	delay(500);
}

void scrollio(){
	for (int positionCounter = 0; positionCounter < 8; positionCounter++) {
		lcd.scrollDisplayLeft();
		delay(100);
	}
		for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
		lcd.scrollDisplayRight();
		delay(100);
	}
	for (int positionCounter = 0; positionCounter < 8; positionCounter++) {
		lcd.scrollDisplayLeft();
		delay(100);
	}
}
void miniScroll(int num, int num2){
	for (int positionCounter = 0; positionCounter < num; positionCounter++) {
		lcd.scrollDisplayLeft();
		delay(200);
	}
		for (int positionCounter = 0; positionCounter < num; positionCounter++) {
		lcd.scrollDisplayRight();
		delay(200);
	}
	for (int positionCounter = 0; positionCounter < num; positionCounter++) {
		lcd.scrollDisplayLeft();
		delay(200);
	}

<<<<<<< HEAD
=======
}

>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
void windowSense2(){
long tempdistance2;	
digitalWrite(trigPin2, LOW);  // Added this line
	delayMicroseconds(2); // Added this line
	digitalWrite(trigPin2, HIGH);
	delayMicroseconds(10); // Added this line
	digitalWrite(trigPin2, LOW);
	duration2 = pulseIn(echoPin2, HIGH);
<<<<<<< HEAD
	distance2 = (duration2 / 2) / 29.1;
	aveSense2.push(distance2);
delay(130);
}

=======
		tempdistance2 = (duration2 / 2) / 29.1;
aveSense2.push(tempdistance2);

		if (tempdistance2 < (aveSense2.mean()+7) && tempdistance2 > (aveSense2.mean()-7)){
distance2 = tempdistance2;
delay(130);
		}
}
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
void windowSense(){
long tempdistance; 
	digitalWrite(trigPin, LOW);  // Added this line
	delayMicroseconds(2); // Added this line
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10); // Added this line
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
<<<<<<< HEAD
	distance = (duration / 2) / 29.1;
	aveSense1.push(distance);
	delay(130);
=======
	tempdistance = (duration / 2) / 29.1;
    	  aveSense1.push(tempdistance);
		if (tempdistance < (aveSense1.mean()+7) && tempdistance > (aveSense1.mean()-7)){
			distance = tempdistance;
		delay(130);
}
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
}

void senseMoveBob(){
		windowSense();
			if (distance >= 200 || distance <= 0){
			Serial.println("ERROR Out of range");
			}
			else if (distance > 2){
			autolowerBob();
			}
			else if (distance <= 2){
			autoraiseBob();
			}
	delay(300);
}
void buzzUP(){

}

<<<<<<< HEAD
void autoraiseBob() {
windowSense();
	if (distance < 70) {
		sendValueToLatch(1);
=======
int raiseBOB(String Command){
autoraiseBob();return 1;}
int lowerBOB(String Command){
  autolowerBob();return 1;}

void raiseBoth(){
		sendValueToLatch(33);
		while (distance < 70 && distance2 < 15) {
windowSense(); windowSense2();		
	}
	sendValueToLatch(0);
	bobPos = 1;
	winPos = 1;
	}

void lowerBoth(){
		sendValueToLatch(6);
		while (distance < 70 && distance2 < 15) {
windowSense(); windowSense2();		
	}
	sendValueToLatch(0);
	}

void autoraiseBob(){
		sendValueToLatch(1);
		if (distance < 70) {
		windowSense();
		Serial.print("dist @"); Serial.println(distance);
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
		while (distance < 70) {
			windowSense();
			Serial.print("dist @"); Serial.println(distance);
		}
		sendValueToLatch(0);
		delay(1000);
	}
}
void autolowerBob(){
<<<<<<< HEAD
	windowSense(); 
	if (distance >2 ) { 
	sendValueToLatch(2); 
  while	(distance >2){
          windowSense();
		Serial.print("dist @");Serial.print(distance);
		}
	sendValueToLatch(0); 
	delay(1000);
=======
	if (distance >5 ) { 
	sendValueToLatch(2); 
	while (distance >5){
		windowSense();
		Serial.print("dist @");Serial.print(distance);
		}
	sendValueToLatch(0); 
	delay(500);
	Serial.println("configuring final height");
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
	}
	windowSense();
if (distance >2 && distance <= 5) {
shortBobD();	
}	
if (distance ==1 || distance ==2) {
		shortBobU();
}
}
 
 void shortBobU(){
sendValueToLatch(1); 
	Serial.println("miniRaising ");
	delay(1500);
sendValueToLatch(0); 
}
void shortBobD(){
sendValueToLatch(2); 
	delay(1500);
	sendValueToLatch(0); 
}
void settiltTime(){
tilttimerS = (tilttimer/1000);
wintimer = (tilttimer*5);
wintimerS = (wintimer/1000);
	}

int pussPull(String command) {
    digitalWrite(piezoPin, HIGH);  delay(1000);	digitalWrite(piezoPin, LOW);	delay(1000);	digitalWrite(piezoPin, HIGH);	delay(500);
	digitalWrite(piezoPin, LOW);	delay(500);	digitalWrite(piezoPin, HIGH);	delay(500);	digitalWrite(piezoPin, LOW);  return 1;
}
int buzz(String command) {     
  buzzUP();return 1;
}

void updateLcd(){
	lcd.setCursor(0, 0); lcd.print("BLIND");
	lcd.setCursor(15, 0);	lcd.print(distance);	lcd.print("cm");
		lcd.setCursor(0,1);	lcd.print("WINDOW");
		lcd.setCursor(15, 1); lcd.print(distance2);	lcd.print("cm");
		int barlevel = map(rm_light, 0, 1023, 0, 100); 
		lcd.setCursor(0, 2); lcd.print("Lux=");  lcd.print(rm_light);  lcd.print(" ~~~ ");lcd.print(barlevel);  lcd.print("%");lcd.setCursor(0, 3);  lcd.print(" Temp:");lcd.print(temperature);lcd.print("C");
//	miniScroll(, 3);
	}

void closeWin() {
<<<<<<< HEAD
	  Serial.println("window forward");
=======
	  Serial.println("window CLOSE");
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
	  sendValueToLatch(4);
	delay(2000);
	sendValueToLatch(0);
	delay(200);
}
<<<<<<< HEAD

void openWin() {
	  Serial.println("window bwd");
=======
void openWin() {
	  Serial.println("window OPEN");
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
	  sendValueToLatch(32);
	delay(2000);
	sendValueToLatch(0);
	delay(200);
}
   
void beep(int num){
for (int i = 0; i < num; i++){
digitalWrite(piezoPin, HIGH);
delay(200);
digitalWrite(piezoPin, LOW);
delay(700);
}
}

void raiseBob(){
  sendValueToLatch(1); 
  Serial.println("RAISING BLIND");
delay(5000);
  sendValueToLatch(0); 
  Serial.println("DONE");
}
void lowerBob(){
	sendValueToLatch(2); 
Serial.println("LOWERING BLIND");
delay(5000);
  sendValueToLatch(0); 
  Serial.println("DONE");
  }

void sendValueToLatch(int latchValue){
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x12);        // Select GPIOA
  Wire.write(latchValue);  // Send value to bank A
  Wire.endTransmission();
}
void storeCode(void) {
  GotNew=true;
  codeType = My_Decoder.decode_type;
  if (codeType == UNKNOWN) {
    Serial.println("Received unknown code, saving as raw");
    rawCount = My_Decoder.rawlen-1;
    for (int i = 1; i <=rawCount; i++) {
      rawCodes[i - 1] = My_Decoder.rawbuf[i];
    };
    My_Decoder.DumpResults();
    codeType=UNKNOWN;
  }
  else {
    Serial.print(F("Received "));
    Serial.print(Pnames(codeType));
    if (My_Decoder.value == REPEAT) {
      // Don't record a NEC repeat value as that's useless.
      Serial.println(F("repeat; ignoring."));
     }
     else {
       codeValue = My_Decoder.value;
       codeBits = My_Decoder.bits;
     }
     Serial.print(F(" Value:0x"));
     Serial.println(My_Decoder.value, HEX);
  }
}

void setluxBar(){
	int barlevel = map(rm_light, 0, 1023, 0, ledCount);
	for (int thisLed = 0; thisLed < ledCount; thisLed++) {
		if (thisLed < barlevel) {
//			digitalWrite(barpins[thisLed], HIGH);
		}
		else {    
//			digitalWrite(barpins[thisLed], LOW);
		}
	}
}
void readAndPrint(){
	readSensors();
	int barlevel = map(rm_light, 0, 1023, 0, ledCount);
	printSensors();
}
void readSensors() {
	lux = lightMeter.readLightLevel();
	rm_light = lux;
	lightPercent = map(rm_light, lightMin, lightMax, 0, 100);
	byte i;
	byte present = 0;
	byte type_s;
	byte data[12];
	byte addr[8];
	if (!ds.search(addr)) {
		//    Serial.println("No more addresses.");
		//    Serial.println("*");
		ds.reset_search();
		delay(250);
		return;
	}
	// Serial.print("ROM =");
	//  for( i = 0; i < 8; i++) {
	//  Serial.write(' ');
	//   Serial.print(addr[i], HEX);
	// }
	if (OneWire::crc8(addr, 7) != addr[7]) {
		Serial.println("CRC is not valid!");
		return;
	}
	// Serial.println();
	// the first ROM byte indicates which chip
	switch (addr[0]) {
	case 0x10:
		//     Serial.println("  Chip = DS18S20");  // or old DS1820
		type_s = 1;
		break;
	case 0x28:
		//      Serial.println("  Chip = DS18B20");
		type_s = 0;
		break;
	case 0x22:
		//     Serial.println("  Chip = DS1822");
		type_s = 0;
		break;
	default:
		//     Serial.println("Device is not a DS18x20 family device.");
		return;
	}

	ds.reset();
	ds.select(addr);
	ds.write(0x44, 1);        // start conversion, with parasite power on at the end
	delay(1000);     // maybe 750ms is enough, maybe not
	// we might do a ds.depower() here, but the reset will take care of it.
	present = ds.reset();
	ds.select(addr);
	ds.write(0xBE);         // Read Scratchpad
	//  Serial.print("  Data = ");
	//  Serial.print(present, HEX);
	//  Serial.print(" ");
	for (i = 0; i < 9; i++) {           // we need 9 bytes
		data[i] = ds.read();
		//   Serial.print(data[i], HEX);
	}
	int16_t raw = (data[1] << 8) | data[0];
	if (type_s) {
		raw = raw << 3; // 9 bit resolution default
		if (data[7] == 0x10) {
			raw = (raw & 0xFFF0) + 12 - data[6];
		}
	}
	else {
		byte cfg = (data[4] & 0x60);
		if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
		else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
		else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
	}
	celsius = (float)raw / 16.0;
	//  Serial.print("  Temperature = ");
	//  Serial.print(celsius);
	// Serial.print(" Celsius, ");
	temp_c = celsius;
	dtostrf(temp_c, 6, 2, tempbuffer);
	temperature = tempbuffer;
	temperature.trim();
	aveRL.push(rm_light);
	aveRT.push(temp_c);
logged++;
}

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
String aiokey = "ff43ff22b3aba7f9aaf9b91b7cb6f950b8deaee9";
// aiokey for adafruit io account

    int minat = 0;
    int maxat = 0;
    int minat2= 0;
    int maxat2 = 0;
IRrecv My_Receiver(2);
#define trigPin2 33    
#define echoPin2 31		
#define trigPin 37     
#define echoPin 35
BH1750 lightMeter;
const int potDialPin = A0;
OneWire  ds(A3);
#define I2C_ADDR  0x20
const int piezoPin = 0;
LiquidTWI lcd(0);

#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,SPI_CLOCK_DIV2);
aREST rest = aREST();
#define LISTEN_PORT           8072
Adafruit_CC3000_Server restServer(LISTEN_PORT);
MDNSResponder mdns;
#define WLAN_SSID       "10011011001101"
#define WLAN_PASS       "4328646517"
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define MY_PROTOCOL 	SONY
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

#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
#define WEBSITE      "io.adafruit.com"
#define WEBPAGE      "/api/groups/envirosensor-feeds/send.json"

uint32_t ip;

int port = 1883;
int maximumRange = 200; int minimumRange = 0; // Minimum range needed
long duration; long distance; // Duration used to calculate distance
long duration2; long distance2; // Duration used to calculate distance

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

Serial1.begin(9600);
Serial1.print("hiBT?");
	Serial.println("LOADING SENSORS");
	pinMode(trigPin, OUTPUT);
	pinMode(trigPin2, OUTPUT);
	pinMode(echoPin, INPUT);
	pinMode(echoPin2, INPUT);	
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
  /*TO ENABLE INTERNET :
 //   wdt_enable(WDTO_4S);wdt_reset();	
		rest.variable("room_temp",&rm_temp);	rest.variable("room_light",&rm_light);      
        rest.function("raiseBob",raiseBOB);	rest.function("lowerBob",lowerBOB);	
	rest.function("buzz",buzz);	  rest.set_id("172");			rest.set_name("RoomBot");

	Serial.println("LOADING WIFI CONNECTION");
  if (!cc3000.begin())  {    while(1);  }  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {    while(1);  }  while (!cc3000.checkDHCP())  {    delay(100);  }  
  if (!mdns.begin("arduino", cc3000)) {   while(1);   }  restServer.begin();
  Serial.println("Online and listening for connections...");  

for lcd:
*/	lcd.begin(20, 4);	lcd.setBacklight(HIGH);lcd.setCursor(0, 2);
	lcd.print("** INITIALISING.. **");

Serial.println("ROOMBOT SETUP COMPLETE");
	My_Receiver.enableIRIn(); // Start the receiver
	Serial.println("Press ZERO for Options"); 
}

void loop() {
time = millis();
if (Serial.available()) Serial1.print(Serial.read());
   if (Serial1.available()) Serial.print(Serial1.read());

   windowSense();
   windowSense2();

	nextup = ((interval + lastup) - time);
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
Serial.println("*");
}

void serialcomms(){
	
	if (Serial.available()){
	int command = Serial.read();
		switch (command)
		{
		case '0':
			Serial.print("BOB@ ");
			Serial.print(distance);
			Serial.print("cm **");
			Serial.print("BOB2@ ");
			Serial.print(distance2);
			Serial.println("cm");
			Serial.println("TSMARTPad- ROOM OPTIONS");
			Serial.println(" Current Sensor Readings: "); readAndPrint();
			Serial.println(" 1/q- Auto raise/lower BOB ");
			Serial.println(" 2/w-  shortBobU/D   3/e- bob norm ");
			Serial.println("4/r-motor2secs g- halfOpen 5/t-SenseOpen/CloseWin   p-pot mode ");
			Serial.println(" 9- SEND2SERVER");
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
case 'g': winHalf(); break;
		case 't': closeSenseWin(); break;
		case '6': sendValueToLatch(32);      Serial.println("Activating relay 6"); break;
			//	sendValueToLatch(16);  Serial.println("Activating relay 5");   break;
		case '7':  sendValueToLatch(64);  Serial.println("Activating relay 7"); break;
		case '8':  sendValueToLatch(128);      Serial.println("Activating relay 8");   break;
		case 'p': 	potDialVal = analogRead(potDialPin);
			Serial.print("pot is");		Serial.println(potDialVal);
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
			break;
		case 'z':sendValueToLatch(0);      Serial.println("Resetting all relays");  break;
		case '9':   send2server();       break;
		}
	}
}
    
	void openSenseWin(){
	windowSense2();
Serial.println("OPENING WINDOW TO 30CM...");
	sendValueToLatch(32); 
while 	(distance2 <30){
                windowSense2();
		Serial.print("dist @");Serial.println(distance2);
        delay(100);
	}
	sendValueToLatch(0); 
	delay(50);
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
}

void closeSenseWin(){
  	windowSense2();
	if (distance2 >3 ) { 
	sendValueToLatch(4); 
while 	(distance2 >4){
                windowSense2();
		Serial.print("dist @");Serial.println(distance2);
        delay(100);
	}
	sendValueToLatch(0); 
	delay(50);
	}
}	

	
	void IRDetected(){
 	 Serial.println("IR COMMAND DETECTED - DECODING...");
	My_Decoder.decode(); GotOne=true;
	GotNew=true;  codeType = My_Decoder.decode_type;  if (codeType == UNKNOWN) {    Serial.println("Received unknown code");
    rawCount = My_Decoder.rawlen-1;    }   
	else{	Serial.print(F("Received "));    Serial.print(Pnames(codeType));
	if (My_Decoder.value == REPEAT) {	Serial.println(F("repeat; ignoring."));     } 
	 else{       codeValue = My_Decoder.value; codeBits = My_Decoder.bits;
} 	Serial.print(F(" Value:0x"));Serial.println(My_Decoder.value, HEX);
if(My_Decoder.decode_type==MY_PROTOCOL) {
Serial.println("Code is for TSMARTPad"); 
lcd.clear(); lcd.setCursor(0,3); lcd.print("**TSMARTPad Code**");
switch(My_Decoder.value) {
case UP_ARROW:     shortBobU();break;
case DOWN_ARROW:    	shortBobD();break;
case SELECT_BUTTON: 	buzzUP();	break;
case SOUND_PREV:	autolowerBob(); break;	// raise room bob
case SOUND_NEXT: autoraiseBob(); break; 	 // lower room bob
//case SUBTITLE: 
case BUTTON_0: senseMoveBob(); break;
case BUTTON_9: sendValueToLatch(0); Serial.println("resetting all relays"); break;
			}
}
else if (My_Decoder.decode_type == MY_PROTOCOL2) {
      //	leds.setLEDs(LED_YELLOW); leds.update();
      Serial.println("Code is IR command for TSMARTPad");
      
      switch (My_Decoder.value) {
      case DOWN_ARROW2:
        printSensors();
        break;
      case UP_ARROW2:printSensors();
        break;
      case LEFT_ARROW2:printSensors();
        break;

      case RIGHT_ARROW2:printSensors();
break;
      }		}
	}	
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

void windowSense2(){
long tempdistance2;	
digitalWrite(trigPin2, LOW);  // Added this line
	delayMicroseconds(2); // Added this line
	digitalWrite(trigPin2, HIGH);
	delayMicroseconds(10); // Added this line
	digitalWrite(trigPin2, LOW);
	duration2 = pulseIn(echoPin2, HIGH);
	distance2 = (duration2 / 2) / 29.1;
aveSense2.push(distance2);
delay(130);
}

void windowSense(){
long tempdistance; 
	digitalWrite(trigPin, LOW);  // Added this line
	delayMicroseconds(2); // Added this line
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10); // Added this line
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	distance = (duration / 2) / 29.1;
    	  aveSense1.push(tempdistance);
	
delay(130);
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

void autoraiseBob() {
	if (distance < 70) {
		sendValueToLatch(1);
		windowSense();
		Serial.print("dist @"); Serial.println(distance);
		while (distance < 70) {
			windowSense();
			Serial.print("dist @"); Serial.println(distance);
		}
		sendValueToLatch(0);
		delay(1000);
	}
	windowSense();
	Serial.print("dist @"); Serial.print(distance);
	if (distance < 70) {
		autoraiseBob();
	}
}
void autolowerBob(){
	if (distance >2 ) { 
	sendValueToLatch(2); 
	windowSense(); 
while 	(distance >2){
  windowSense();
		Serial.print("dist @");Serial.print(distance);
		}
	sendValueToLatch(0); 
	delay(50);
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

void send2server(){
	Serial.println("Connecting....");
	 String request = "GET io.adafruit.com/api/groups/envirosensor-feeds/send.json?x-aio-key="	
+ aiokey + "&rm_temp=" + aveRT.mean() +"&stddev="+ aveRT.stddev() + "&samplesize=" + logged + " HTTP/1.0";

 send_request(request);	Serial.print("request: ");	Serial.println(request);	Serial.println("Temp Data SENT");
delay(20);

  String request2 = "GET io.adafruit.com/api/groups/envirosensor-feeds/send.json?x-aio-key="
+ aiokey + "&rm_light=" + aveRL.mean() + "&stddev=" + aveRL.stddev() + "&samplesize=" + logged + " HTTP/1.0";
send_request(request2);Serial.print("request2: ");	Serial.println(request2);	Serial.println("Light Data SENT");
    }

	void send_request(String req) {
  Adafruit_CC3000_Client www = cc3000.connectTCP(ip, 80);
  if (www.connected()) {
    www.print(req);
    www.println(F(""));
	  } else {
    Serial.println(F("Connection failed"));    
   
  }

  Serial.println(F("-------------------------------------"));
  
  /* Read data until either the connection is closed, or the idle timeout is reached. */ 
  unsigned long lastRead = millis();
  while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (www.available()) {
      char c = www.read();
      Serial.print(c);
      lastRead = millis();
    }
  }
  www.close();
  Serial.println(F("-------------------------------------"));
  
  /* You need to make sure to clean up after yourself or the CC3000 can freak out */
  /* the next time your try to connect ... */
  Serial.println(F("\n\nDisconnecting"));
  cc3000.disconnect();
  
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

int raiseBOB(String Command){
autoraiseBob();return 1;}
int lowerBOB(String Command){
  autolowerBob();return 1;}


void updateLcd(){
		lcd.setCursor(0, 0); lcd.print("BOB@ ");
	lcd.setCursor(15, 0);	lcd.print(distance);	lcd.print("cm");
		lcd.setCursor(0,1);	lcd.print("BOB2@");
		lcd.setCursor(15, 1); lcd.print(distance2);	lcd.print("cm");
		lcd.setCursor(0, 2);  lcd.print("Lux=");  lcd.print(rm_light);  lcd.print("*Temp=");  lcd.print(temperature);lcd.print("C");
	lcd.setCursor(0, 3); lcd.print("T-");  lcd.print(time); lcd.print("*NXT-");  lcd.print(nextup);
	}

void closeWin() {
	  Serial.println("window CLOSE");
	  sendValueToLatch(4);
	delay(2000);
	sendValueToLatch(0);
	delay(200);
}

void openWin() {
	  Serial.println("window OPEN");
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


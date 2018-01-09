/* ROOMeDUINO MEGA 
Sourcetree GIT Edition
30/8

Relay8 #s:
1-Bob
2-BobRev
3-M2
4-lIGHTS1
5-LIGHTS2
6-M2Rev
7- *empty*
8-Buzzer
 */
#include <Stepper.h>
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

const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 43,45,47,49);            
// RED (COIL A)
// GRN (COIL C)		(COIL B)	(COIL D)
// 					YELLOW		BLUE

BH1750 lightMeter;
const int potDialPin = A15;
OneWire  ds(A3);  // on pin 10 (a 4.7K resistor is necessary)
#define trigPin 26      // next to VCC
#define echoPin 28    // next to GND
#define I2C_ADDR  0x20
int relayNo;
IRrecv My_Receiver(2);
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

unsigned long interval = 300000;           // every 5 minutes
unsigned long time; long nextup; long lastup;
	int tilttimer = 2000; 	//default length of "tilt fwd or bwd'
	int tiltmax = 10000;      
	int tiltmin = 1000;
	int testtimer = 1000;
	int tilttimerS = (tilttimer/1000);
	int wintimer = (tilttimer*5);
	int wintimerS = (wintimer/1000);

IRTYPES codeType;          // The type of code
unsigned long codeValue;   // The data bits if type is not raw
int codeBits;              // The length of the code in bits
unsigned int rawCodes[RAWBUF]; // The durations if raw
int rawCount;                   //The number of interval samples
bool GotOne, GotNew; IRdecode My_Decoder; 

int rm_light; uint16_t lux; int lightPercent;
int rm_temp; String temperature; float celsius;float temp_c;
int lightMin =0;int lightMax = 1023;
int potVal = 0;int potDialVal = 0; 
static char tempbuffer[10];int prevPot = 0;
Average<float> aveRT(10);Average<float> aveRL(10);int logged = 0;

uint32_t ip = cc3000.IP2U32(192,168,0,110);//your computer's ip address
int port = 80;String repository = "/energy_project/";
int maximumRange = 200; int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t retarrow[8] = {	0x1,0x1,0x5,0x9,0x1f,0x8,0x4};

int barlevel; const byte barpins[10] = {23,25,27,29,31,33,35,37,39,41};
const int ledCount = 10;

void setup(){
	Serial.begin(115200);
	Serial.println("ROOMBOT INITIALISING...");
	lcd.begin(20, 4);	lcd.setBacklight(HIGH);lcd.setCursor(0, 2);
	lcd.print("** INITIALISING.. **");

	pinMode(trigPin, OUTPUT);pinMode(echoPin, INPUT);
  
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(barpins[thisLed], OUTPUT); 
  digitalWrite(barpins[thisLed], HIGH);
  }
 GotOne=false; GotNew=false;codeType=UNKNOWN;   codeValue=0; 
 	
 pinMode(piezoPin, OUTPUT);pinMode(potDialPin, INPUT);

  Wire.begin();  Wire.beginTransmission(I2C_ADDR);  Wire.write(0x00); 
  Wire.write(0x00); // Set all of bank A to outputs
  Wire.endTransmission();

  myStepper.setSpeed(15);
  /*
  lightMeter.begin(); 
  rm_light = lux;	rest.variable("rm_temp",&rm_temp);	rest.variable("rm_light",&rm_light);       
        rest.function("raiseBob",raiseBOB);
	rest.function("lowerBob",lowerBOB);
	rest.function("pussPull",pussPull);
	rest.function("buzz",buzz);
	rest.set_id("172");	rest.set_name("RoomBot");

	Serial.println("LOADING WIFI CONNECTION");
  
  if (!cc3000.begin())  {    while(1);  }  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {    while(1);  }  while (!cc3000.checkDHCP())  {  delay(100);  }    if (!mdns.begin("arduino", cc3000)) {   while(1);   }  restServer.begin();
  Serial.println("Online and listening for connections...");  
lcd.clear();

for (int thisLed = 0; thisLed < ledCount; thisLed++) {
digitalWrite(barpins[thisLed], LOW);
}
My_Receiver.enableIRIn(); // Start the receiver
*/
Serial.println("ROOMBOT SETUP COMPLETE");
Serial.println("Press ZERO for Options");
}

void loop(){

  
int stepCount = 0;  // number of steps the motor has taken
  myStepper.step(stepsPerRevolution);
  delay(500);
  
  
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500); 
/*




	wdt_enable(WDTO_8S);wdt_reset();
	Serial.print(".");
	time = millis();
	//settiltTime();//kitttheBar();
	nextup = ((interval + lastup) - time);
	windowSense();
	readSensors();
	updateLcd();
	setluxBar();
	wdt_reset();
	mdns.update();
	Adafruit_CC3000_ClientRef client = restServer.available();
	rest.handle(client);

	if (time>(lastup+interval)){
		Serial.println("Time to send server");
		readAndPrint();
		beep(1);
		send2server();
		delay(100);
		lastup = time;
		}
wdt_reset();
wdt_disable();

if (My_Receiver.GetResults(&My_Decoder)) {
//        blinkalt();
			IRDetected();
			delay(100);
			My_Receiver.resume();
		}
*/
	serialcomms();
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

void windowSense(){
	digitalWrite(trigPin, LOW);  // Added this line
	delayMicroseconds(2); // Added this line
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10); // Added this line
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	distance = (duration / 2) / 29.1;
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
int activateRelayNo(int relayNo){
  	if( relayNo == '0' ){
      sendValueToLatch(0); Serial.println("Resetting all relays");
    }
    if( relayNo == '1' ){
      sendValueToLatch(1);      Serial.println("Activating brelay 1");
    }
    if( relayNo == '2' ){
      sendValueToLatch(2);      Serial.println("Activating relay 2");
    }
    if( relayNo == '3' ){
      sendValueToLatch(4);      Serial.println("Activating relay 3");
    }
    if( relayNo == '4' ){
      sendValueToLatch(8);      Serial.println("Activating relay 4");
    }
    if( relayNo == '5' ){
      sendValueToLatch(16);      Serial.println("Activating relay 5");
    }
    if( relayNo == '6' ){
      sendValueToLatch(32);      Serial.println("Activating relay 6");
    }
    if( relayNo == '7' ){
      sendValueToLatch(64);      Serial.println("Activating relay 7");
    }
    if( relayNo == '8' ){
      sendValueToLatch(128);      Serial.println("Activating relay 8");
    }
    //if( relayNo == '9' ){ sendValueToLatch(255); Serial.println("Activating ALL relays");}
  }

void sendValueToLatch(int latchValue){
Wire.beginTransmission(I2C_ADDR);
Wire.write(0x12);       Wire.write(latchValue);  Wire.endTransmission();
}

void buzzUP(){
       Serial.println(" COMMENCING BUZZING @ INTERCOM...  ");
activateRelayNo(8);
delay(800);
activateRelayNo(0);
Serial.println(" *** ");
delay(500);
	activateRelayNo(8);
	delay(500);
	activateRelayNo(0);       
	delay(500);
		activateRelayNo(8);
		delay(1000);
		Serial.println(" *** *** ");
       	activateRelayNo(0);
		delay(200);
}

void testM2(){
activateRelayNo(3);
delay(2000);
activateRelayNo(0);	
delay(2000);
activateRelayNo(3);
delay(2000);
activateRelayNo(0);
delay(200);
}

void testM2rev(){
activateRelayNo(6);
delay(2000);
activateRelayNo(0);		
delay(2000);
activateRelayNo(6);
delay(2000);
activateRelayNo(0);
delay(200);	
}

void autoraiseBob(){
	windowSense();
	while (distance <90) {
		sendValueToLatch(1); 
		windowSense();
		}
	sendValueToLatch(0); 	
	delay(50);
	}
	
void autolowerBob(){
	windowSense();
	while (distance >2 ) { 
		sendValueToLatch(2); 
		windowSense(); 
		}
	sendValueToLatch(0); 
	delay(50);
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
	
	String request = "GET " + repository + "sensor.php?rm_temp=" + aveRT.mean() +","+ aveRT.stddev() + "," + logged + " HTTP/1.0";
	send_request(request);	Serial.print("request: ");
		Serial.println(request);	Serial.println("Temp Data SENT");
		
		delay(20);
	String request2 = "GET " + repository + "sensor.php?rm_light=" + aveRL.mean() + "," + aveRL.stddev() + "," + logged + " HTTP/1.0";
	send_request(request2);Serial.print("request2: ");	Serial.println(request2);	Serial.println("Light Data SENT");
    }
void send_request(String req) {
	Serial.println("Attempting connection to server...");
	Adafruit_CC3000_Client client = cc3000.connectTCP(ip, port);
	if (client.connected()) {
		client.println(req);
			client.println(F(""));
	}
	else {
		Serial.println("Connection failed");
	}
	while (client.connected()) {
		while (client.available()) {
			char c = client.read();
                        Serial.print(c);
		}
	}
	Serial.println("Closing connection");
	Serial.println("");
	client.close();
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

void IRDetected(){
	My_Decoder.decode(); GotOne=true;
	GotNew=true;  codeType = My_Decoder.decode_type;  if (codeType == UNKNOWN) {    Serial.println("RUC @ IR");
    rawCount = My_Decoder.rawlen-1;    }   
	else{	Serial.print(F("Received "));    Serial.print(Pnames(codeType));
	if (My_Decoder.value == REPEAT) {	Serial.println(F("repeat; ignoring."));     } 
	 else{       codeValue = My_Decoder.value; codeBits = My_Decoder.bits;
} 	Serial.print(F(" Value:0x"));Serial.println(My_Decoder.value, HEX);
if(My_Decoder.decode_type==MY_PROTOCOL) {
	
 	 lcd.clear();lcd.setCursor(0,3);lcd.print("IR DECTECTED!!"); scrollio();
			Serial.println("IR COMMAND DETECTED - DECODING...");
//lcd.clear(); lcd.setCursor(0,3); lcd.print("**TSMARTPad Code**");
switch(My_Decoder.value) {
	case SELECT_BUTTON: 	buzzUP();	break;
case SOUND_PREV:	autolowerBob(); break;	// raise room bob
case SOUND_NEXT: autoraiseBob(); break; 	 // lower room bob
//case SUBTITLE: 
case BUTTON_0: senseMoveBob(); break;
case BUTTON_9: sendValueToLatch(0); Serial.println("resetting all relays"); break;
}
}lcd.clear(); lcd.setCursor(0,3); lcd.print("**TSMARTPad Code**");
switch(My_Decoder.value) {
case BUTTON_9: lowerBob(); break;
		}
		}	
}

void updateLcd(){
		lcd.setCursor(0, 0); lcd.print("BOB@ ");
	lcd.setCursor(15, 0);	lcd.print(distance);	lcd.print("cm");
		lcd.setCursor(0,1);	lcd.print("Next@");
		lcd.setCursor(15, 1); lcd.print(nextup);
		lcd.setCursor(0, 2);  lcd.print("Time-");  lcd.print(time); lcd.print("*Last-");  lcd.print(lastup);
	lcd.setCursor(0, 3); lcd.print("*Lux=");  lcd.print(rm_light);  lcd.print("**Temp=");  lcd.print(temperature);lcd.print("C");
	}
	
void raiseBob2(){
	Serial.println("DONE");
  }
	
void lowerBob2(){
  Serial.println("DONE");
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


void serialcomms(){

Serial.println("POTSTEP");
	   int sensorReading = analogRead(A15);
  // map it to a range from 0 to 100:
  int motorSpeed = map(sensorReading, 0, 1023, 0, 100);
  // set the motor speed:
  if (motorSpeed > 0) {
		myStepper.setSpeed(motorSpeed);
    // step 1/100 of a revolution:
    myStepper.step(stepsPerRevolution/100);
	} 

 if (Serial.available()){
int command = Serial.read();
switch (command)
{
case '0':
Serial.println("SMARTPad- ROOM OPTIONS");Serial.println(" Current Sensor Readings: ");printSensors();
			Serial.println(" 1/q-  autoraise/lower BOB ");
			Serial.println(" 2/w-  shortBobD/U  ");
			Serial.println(" 3/e- M2	");
				Serial.println(" 4/5- Relays 4 / 5 lights");
			Serial.println(" s- readAndPrint Sensors"); 
			Serial.println(" 8- buzzUp on relay #8");
	Serial.println(" 9- SEND2SERVER");
			Serial.println(" r- reset all relays ");
	delay(100);			break;

case '1':    autoraiseBob();	break;
case 'q': autolowerBob(); break;
case '2':	shortBobU();	break;
case 'w':shortBobD(); break;
case '3': testM2(); break;
case 'e':testM2rev(); break;
case 's': readAndPrint(); break;
case '4':      activateRelayNo(4);break;
   case '5': activateRelayNo(5); break;
	   case '7':   break;
	case '8': buzzUP(); break;
	case 'r':sendValueToLatch(0); break;
case '9': send2server();   break;
	}
    }
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
	barlevel = map(rm_light, 0, 1023, 0, ledCount);
	for (int thisLed = 0; thisLed < ledCount; thisLed++) {
		if (thisLed < barlevel) {
			digitalWrite(barpins[thisLed], HIGH);
		}
		else {    
			digitalWrite(barpins[thisLed], LOW);
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

void printSensors(){
	Serial.print("Temperature: ");
	Serial.print(temperature);
	Serial.println("C ");
	Serial.print("Light=");  Serial.print(rm_light);  Serial.print("(lux)/");  Serial.print(barlevel);  Serial.println("% ");
	Serial.println(" **  ");
	Serial.println(" Lux Stats");
	// And show some interesting results.
	Serial.print("Mode:   "); Serial.println(aveRL.mode());
	//   Serial.print("Max:    "); Serial.println(aveRL.maximum(&maxat));
	//    Serial.print(" at:    "); Serial.println(maxat);
	//   Serial.print("Min:    "); Serial.println(aveRL.minimum(&minat));
	//  Serial.print(" at:    "); Serial.println(minat);
	Serial.print("StdDev: "); Serial.println(aveRL.stddev());
	Serial.println("Temp Stats");
	Serial.print("Mean:   "); Serial.println(aveRT.mean());
	Serial.print("Mode:   "); Serial.println(aveRT.mode());
	//   Serial.print("Max:    "); Serial.println(aveRT.maximum(&maxat));
	//    Serial.print(" at:    "); Serial.println(maxat);
	//   Serial.print("Min:    "); Serial.println(aveRT.minimum(&minat));
	//  Serial.print(" at:    "); Serial.println(minat);
	Serial.print("StdDev: "); Serial.println(aveRT.stddev());
	Serial.print("Sample Size:"); Serial.println(logged);
	Serial.print("BOB DISTANCE @ ");Serial.print(distance);	Serial.println("cm");
	delay(500);
}
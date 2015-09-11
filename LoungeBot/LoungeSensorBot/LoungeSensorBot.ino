/*** LOUNGEBOT @ TSMARTPad #171
 * 192.168.0.171
 * 7/08 edition  *the sync.
 * 
 * # EVENS ARE LED BAR GRAPH PINS
 * # FOR Kitch WINDOWSENSE
 * 
 ***/

#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <stdlib.h>
#include <OneWire.h>
#include <CC3000_MDNS.h>
#include <aREST.h>
#include <avr/wdt.h>
#include <IRLib.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <BH1750.h>
#include <Average.h>
#include <LiquidTWI.h>
#include <FTRGBLED.h>
#include <Adafruit_NeoPixel.h>


// VARIABLES 	
//i. Sensors
OneWire ds(A1);  
BH1750 lightMeter;
IRrecv My_Receiver(A0);

#define KWtrigPin 0
#define KWechoPin 0
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

const int piezoPin = A8;

#define NEOPIN A15
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, NEOPIN, NEO_GRB + NEO_KHZ800);



const int potDialPin = A13;
 const int potPin = A14;
 const int loungeWinF = 0;// NOW DONE BY V1 SHIELD IN LOUNGE
 const int loungeWinR = 0;  // NOW DONE BY V1 SHIELD IN LOUNGE
 int potVal = 0;
 int potDialVal;
  #define REDPIN 6
 #define GREENPIN 7
 #define BLUEPIN 2
 #define FADESPEED 5   
 int r, g, b;
 
const int num_leds = 1;const int PIN_CKI = 12;const int PIN_SDI = 13;RGBLEDChain leds(num_leds, PIN_CKI, PIN_SDI);
const byte rleds[] = {  0};//22,23,24,25};
const byte gleds[] = {  0};//26,27,28,29};
const byte bleds[] = {  37,39, A3, 41};
const int ledCount = 10; 
const byte aleds[] = {  0};// 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33 };

#define LISTEN_PORT           8071
#define ADAFRUIT_CC3000_IRQ   3		// Define CC3000 chip pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
#define WLAN_SSID       "10011011001101"        // cannot be longer than 32 characters!
#define WLAN_PASS       "4328646517"
#define WLAN_SECURITY   WLAN_SEC_WPA2 // This can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
aREST rest = aREST();// The port to listen for incoming TCP connections 
Adafruit_CC3000_Server restServer(LISTEN_PORT);
MDNSResponder mdns;

// lux bar pin
int barpins[] = { 23, 25, 27, 29, 31, 33, 35, 37, 39,41 };   // an array of pin numbers to which LEDs are attached

int barlevel;
int ledmin = 100;        // sets the max speed (0 = fast) the lower the number the faster it
int ledmax = 200;      // sets the min speed (100 = slow) the higher the number the slower it can go

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,SPI_CLOCK_DIV2);
uint32_t ip = cc3000.IP2U32(192,168,0,110);
int port = 80;
String repository = "/energy_project/";

Adafruit_MotorShield AFMSi = Adafruit_MotorShield(0x61);
Adafruit_DCMotor *KitchenTilt = AFMSi.getMotor(4);
Adafruit_DCMotor *LoungeTilt = AFMSi.getMotor(1);
Adafruit_DCMotor *WindowTilt = AFMSi.getMotor(3);
Adafruit_DCMotor *DoorWindowTilt = AFMSi.getMotor(2);
/*Adafruit_MotorShield AFMSi1 = Adafruit_MotorShield(0x60);
Adafruit_DCMotor *KitchenTilt1 = AFMSi.getMotor(4);
Adafruit_DCMotor *LoungeTilt1 = AFMSi.getMotor(1);
Adafruit_DCMotor *WindowTilt1 = AFMSi.getMotor(3);
Adafruit_DCMotor *DoorWindowTilt1 = AFMSi.getMotor(2);
*/
//Servo Servo1;

unsigned long interval = 300000;           // every minute interval at which to send sensor data to server
unsigned long time;long nextup; long lastup;

#define MY_PROTOCOL 	SONY
#define MY_PRO2COL 	NEC
#define UP_ARROW2      	0x61D6D827 //	INCREASE TILT TIME +1SEC
#define DOWN_ARROW2    	0x61D658A7 //DECREASE TILT TIME -1SEC
#define RIGHT_ARROW2   	0x61D6609F //Move All tilt FWD
#define LEFT_ARROW2    	0x61D620DF //Move All tilt BKD
#define SELECT_BUTTON2 	0x61D6A05F //	BUZZER!!
#define UP_ARROW      	0x1E108 //	INCREASE TILT TIME +1SEC
#define SUBTITLE		0x88108
#define DOWN_ARROW    	0x9E108 //DECREASE TILT TIME -1SEC
#define RIGHT_ARROW   	0xDE108 //Move All tilt FWD
#define LEFT_ARROW    	0x5E108 //Move All tilt BKD
#define SELECT_BUTTON 	0x3E108 //	BUZZER!!
#define BUTTON_0 		0x90108  			//  ACTIVATE SOLENOID @ DOOR		0xA010C?
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

int sevSegtimeS = (time/10000);
int tilttimer = 2000; 	//default length of "tilt fwd or bwd'
int tiltmax = 10000;
int tiltmin = 1000;
int testtimer = 1000; 
int sevSegtime = time;
int tilttimerS = (tilttimer / 1000);
int wintimer = (tilttimer * 5);
int wintimerS = (wintimer / 1000);

int logged;int lg_light; int lg_temp;
const int lightMin = 0;const int lightMax = 1023;
int lightPercent;float celsius; float temp_c; 
String temperature; uint16_t lux;
Average<float> aveLL(10);static char tempbuffer[10];
Average<float> aveLT(10);

byte Tab[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff}; //0,1,2,3,4,5,6,7,8,9, ALL OFF
byte Taf[] = {0xA0, 0x83, 0xa7, 0xa1, 0x86, 0x8e, 0xc2, 0x8b, 0xe6, 0xe1, 0x89, 0xc7, 0xaa, 0xc8, 0xa3, 0x8c, 0x98, 0xce, 0x9b, 0x87, 0xc1, 0xe3, 0xd5, 0xb6, 0x91, 0xb8};//a,b,c,d,e,f,g,h,i,j,k,l,o,m,n,o,p,q,r,s,t,u,v,w,x,y,z
byte Tap[] = {0xff, 0x7f}; //"space", "."
const int latchPin = A12;      //Pin connected to latch pin7SEG
const int clockPin = A13;
const int dataPin = A14;

IRTYPES codeType;      unsigned long codeValue;int codeBits; unsigned int rawCodes[RAWBUF];int rawCount; bool GotOne, GotNew;
IRdecode My_Decoder;
//int num;		// for BEEP function

#define I2C_ADDR  0x20

void setup(){
  Serial.begin(115200);
  Serial.println("TLOUNGE INITIALISING...");
  //Serial2.begin(9600);
  // set the contrast-- 200 is a good middle place to try out
//Serial2.write(0xFE);  Serial2.write(0x50);  Serial2.write(200);
 // delay(10);       
    // set the brightness - we'll max it (255 is max brightness)
  //Serial2.write(0xFE);  Serial2.write(0x99);  Serial2.write(255);
  //delay(10);
  //backlight to Red
 // Serial2.write(0xFE);Serial2.write(0xD0);Serial2.write(0x255);Serial2.write(0x0); Serial2.write(0x0);	
 // delay(10);
  
 // strip.begin();  strip.setBrightness(255);	 strip.show(); 	
  //colorWipe(strip.Color(255, 0, 0), 50);    // red?
  		
  //Serial2.print("TrendySMARTPad -- Connecting?");
 /* pinMode(dataPin, OUTPUT);  
 pinMode(clockPin, OUTPUT);
 pinMode(latchPin, OUTPUT);
  pinMode(KWtrigPin, OUTPUT);
  pinMode(KWechoPin, INPUT);
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(barpins[thisLed], OUTPUT); digitalWrite(barpins[thisLed], LOW);
  }
  clearsevSeg();
  
   for (byte count = 0; count < 4; count++) {
   	pinMode(rleds[count], OUTPUT);   		pinMode(gleds[count], OUTPUT);   		digitalWrite(gleds[count], HIGH);   		digitalWrite(rleds[count], HIGH);
   	}
   */	for (byte count1 = 0; count1 < 4; count1++) {
   	pinMode(bleds[count1], OUTPUT);delay(300);
         }
   	//pinMode(potPin, INPUT); pinMode(potDialPin, INPUT); 	   
	
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(barpins[thisLed], OUTPUT); 
	digitalWrite(barpins[thisLed], HIGH);
	delay(200);
  }
  
  GotOne = false; GotNew = false;   codeType = UNKNOWN;  codeValue = 0;
Serial1.begin(9600);	
	Serial1.println("hello on BT?");
	if (Serial1.available()){
   	Serial.println("Bluetooth Client Online");
   }   else {
   	Serial.println("BT Not Found");
   }
//   pinMode(piezoPin, OUTPUT);
leds.begin();   leds.setLEDs(LED_RED);   leds.update();
  AFMSi.begin();			 

  /*Start your Engines...
 // LoungeTilt->setSpeed(255);
 // LoungeTilt->run(RELEASE);
  //KitchenTilt->setSpeed(255);
  KitchenTilt->run(RELEASE);
//  DoorWindowTilt->setSpeed(255);
 // DoorWindowTilt->run(RELEASE);
  WindowTilt->setSpeed(255);
  WindowTilt->run(RELEASE);
  */
  lightMeter.begin();
  lux = lightMeter.readLightLevel();
  lg_light = lux;
  Serial.println("initiALISING WiFi");

  rest.variable("lg_light", &lg_light);
  rest.variable("lg_temp", &lg_temp);
  //rest.function("raiseKW",raiseKW);
  //rest.function("lowerKW",lowerKW);
  //rest.function("tiltFW",tiltFW);		// Function to be exposed
  //rest.function("tiltBK",tiltBK);	
  rest.set_id("171");			  			// Give name and ID to device
  rest.set_name("LoungeBot");

  if (!cc3000.begin())  {    
    while(1);  
  }  
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {    
    while(1);  
  }  
  while (!cc3000.checkDHCP())  {    
    delay(100);  
  }  
 
  //wdt_reset();
  if (!mdns.begin("arduino", cc3000)) {   
    while(1);   
  }  
  restServer.begin();
  
  Serial.println("Connected to WiFi network");
  Serial.println("MEGA IR SETUP COMPLETE");
  Serial.println("Press ZERO for Options");
Serial2.write(0xFE);Serial2.write(0xD0);Serial2.write(0x0);Serial2.write(0x0); Serial2.write(0x255);	// blue
delay(10);
//clearTTL();
//Serial2.write("CONNECTED!");
  // Wire.begin();
   //rainbowCycle(20);
	My_Receiver.enableIRIn();  leds.setLEDs(LED_BLUE);   leds.update();
}

void loop(void){
   //   colorWipe(strip.Color(0, 0, 255), 50);   
	  Serial.print(".");  
//if (Serial.available()) Serial1.print(Serial.read());
   // if (Serial1.available()) Serial.print(Serial1.read());
    readSensors();
   time = millis();
  nextup = ((interval + lastup) - time);
  
 // settiltTime();
  setluxBar();


  mdns.update();
  Adafruit_CC3000_ClientRef client = restServer.available();
  rest.handle(client);
kitttheBar();
  if (time > (lastup + interval)){
    Serial.println("TIME TO SEND 2 SERVER");	
    readAndPrint();
    printAverage();
    leds.setLEDs(LED_GREEN);     leds.update();
        send2server();
    sevSegPrint(String(temperature));	
        lastup = time;
  }

 // if (My_Receiver.GetResults(&My_Decoder)) {
//knightrider();
  //  theaterChaseRainbow(50);
 //     clearsevSeg();	
//kitttheBar();
  //  delay(100);
//My_Receiver.resume();
 // }    
  serialcomms();
}
/*
int lowerKW(String Command){
  kitchWinFwd();
  return 1;
}
int raiseKW(String Command){
  kitchWinBwd();
  return 1;
}
int tiltFW(String Command){
  tiltfwd();
  return 1;
}
int tiltBK(String Command){
  tiltbwd();
  return 1;
}

void sendValueToLatch(int latchValue){
Wire.beginTransmission(I2C_ADDR);
Wire.write(0x12);        // Select GPIOA
Wire.write(latchValue);  // Send value to bank A
Wire.endTransmission();
}
void tiltfwd(){
  blink(rleds[0], tilttimerS);
  blink(gleds[0], tilttimerS);
  Serial.println("ALL BWD"); 
  WindowTilt->run(BACKWARD); 
  KitchenTilt->run(BACKWARD); 
  DoorWindowTilt->run(BACKWARD);
  delay(tilttimer);	
  WindowTilt->run(RELEASE);  			
  DoorWindowTilt->run(RELEASE);	
  LoungeTilt->run(RELEASE); 			
  KitchenTilt->run(RELEASE);
}
void tiltbwd(){
  blink(rleds[3],tilttimerS);
  blink(gleds[3], tilttimerS);
  readSensors();		
  Serial.println(" ALL FWD");		
  WindowTilt->run(FORWARD);			
  KitchenTilt->run(FORWARD);		
  DoorWindowTilt->run(FORWARD);    			
  LoungeTilt->run(FORWARD);		
  delay(tilttimer);			
  WindowTilt->run(RELEASE);  			
  DoorWindowTilt->run(RELEASE);			
  LoungeTilt->run(RELEASE);	
  KitchenTilt->run(RELEASE);			
}
*/
void IRDetected(){
  kitttheBar();
  Serial.println("IR COMMAND DETECTED - DECODING...");
}
/*My_Decoder.decode();
 GotOne = true;
  	leds.setLEDs(LED_VIOLET); leds.update();
  codeType = My_Decoder.decode_type;
  if (codeType == UNKNOWN) {
    Serial.println("Received unknown code, no known protocol");
    rawCount = My_Decoder.rawlen-1;
    for (int i = 1; i <=rawCount; i++) {
      rawCodes[i - 1] = My_Decoder.rawbuf[i];
    }
    My_Decoder.DumpResults();
    codeType=UNKNOWN;
  }
  else{
    Serial.print("Received ");
    Serial.print(Pnames(codeType));
    if (My_Decoder.value == REPEAT) {	// Don't record a NEC repeat value as that's useless.
      //Serial.println(F("repeat; ignoring."));
    } 
    else {       
      codeValue = My_Decoder.value;
      codeBits = My_Decoder.bits;
    }
    Serial.print(F(" Value:0x"));
    Serial.println(My_Decoder.value, HEX);

    if (My_Decoder.decode_type == MY_PROTOCOL) {
	leds.setLEDs(LED_YELLOW); leds.update();
      Serial.println("Code is IR command for TSMARTPad");
      beep(1);
      switch (My_Decoder.value) {
      case DOWN_ARROW:
        tilttimer = max(tilttimer - 1000, tiltmin); 
        Serial.println(""); 
        Serial.print("Tilt duration has been changed to "); 
        Serial.print(tilttimerS); 
        Serial.println("Seconds"); 
        Serial.print("...whilst WindowAction Duration now @"); 
        Serial.print(wintimerS); 
        Serial.println("Seconds"); 
        beep(tilttimerS); 
        break;
      case UP_ARROW:
        tilttimer = min(tilttimer + 1000, tiltmax); 
        Serial.println(""); 
        Serial.print("Tilt duration has been changed to "); 
        Serial.print(tilttimer / 1000); 
        Serial.println("Seconds"); 
        Serial.print("...whilst WindowAction Duration now @"); 
        Serial.print(wintimerS); 
        Serial.println("Seconds"); 
        beep(tilttimerS); 
        break;
      case LEFT_ARROW:
        blink(rleds[0], tilttimerS);
        blink(gleds[0], tilttimerS);
        Serial.println("ALL BWD"); 
        WindowTilt->run(BACKWARD); 
        KitchenTilt->run(BACKWARD); 
        DoorWindowTilt->run(BACKWARD);
        delay(tilttimer);	
        WindowTilt->run(RELEASE);  			
        DoorWindowTilt->run(RELEASE);	
        LoungeTilt->run(RELEASE); 			
        KitchenTilt->run(RELEASE);	
        break;

      case RIGHT_ARROW:
        blink(rleds[3], tilttimerS);
        blink(gleds[3], tilttimerS);
        readSensors();		
        Serial.println(" ALL FWD");
        WindowTilt->run(FORWARD);			
        KitchenTilt->run(FORWARD);
        DoorWindowTilt->run(FORWARD);    			
        LoungeTilt->run(FORWARD);
        delay(tilttimer);			
        WindowTilt->run(RELEASE);  			
        DoorWindowTilt->run(RELEASE);			
        LoungeTilt->run(RELEASE);
        KitchenTilt->run(RELEASE);			
        break;

      case BUTTON_0:
        Serial.println("rec - sensors"); 
        readAndPrint(); 
        break;

      case BUTTON_1:
        Serial.println(" Direction: FORWARD - KitchenTilt ... ");
        KitchenTilt->run(FORWARD);
        delay(tilttimer);
        KitchenTilt->run(RELEASE);
        delay(30); // 'coast' for 1/10 second		
        break;

      case BUTTON_2:
        Serial.println(" WindowTILT ...");
        WindowTilt->run(FORWARD);
        delay(tilttimer); // 'coast' for 1/10 second
        WindowTilt->run(RELEASE);
        break;

      case BUTTON_3:
        Serial.println(" DoorWindowTILT ... 5sec @ 255 BKD");
        DoorWindowTilt->run(FORWARD);
        delay(tilttimer); // 'coast' for 1/10 second
        DoorWindowTilt->run(RELEASE);
        break;

      case BUTTON_4:
        Serial.println(" Direction: BACKWARD - KitchenTilt ... ");
        KitchenTilt->run(BACKWARD);
        delay(tilttimer);
        KitchenTilt->run(RELEASE);
        break;

      case BUTTON_5:
        Serial.println(" WindowTILT ...");
        WindowTilt->run(BACKWARD);
        delay(tilttimer); // 'coast' for 1/10 secon
        WindowTilt->run(RELEASE);
        break;
      case BUTTON_6:
        Serial.println(" DoorWindowTILTING");
        DoorWindowTilt->run(BACKWARD);
        delay(tilttimer); // 'coast' for 1/10 second
        readSensors();
        DoorWindowTilt->run(RELEASE);
        break;
      case BUTTON_7:
        Serial.println(" Tilting Lounge Blind");
        LoungeTilt->run(FORWARD);
        delay(tilttimer);
        LoungeTilt->run(RELEASE);
        break;

      case BUTTON_8:   	
        Serial.println("rec"); 	
        LoungeTilt->run(BACKWARD);	
        delay(tilttimer); 	
        readSensors();	
        LoungeTilt->run(RELEASE);  	
        break;

      case BUTTON_9:
        Serial.println("REC");
        break;
        // FURTHER COMMANDS IR BETA V2
      case YELLOW:
        Serial.println("CLOSING KITCHEN WINDOW"); 
        kitchWinBwd();
        break;
      case BLUE:
        Serial.println("CLOSING KITCHEN WINDOW"); 	
        kitchWinFwd();
        break;
      case GREEN:
        Serial.println("GREEN  "); 
        gedon();	
        Serial3.println("");
        redoff();		
        break;
      case RED:
        gedoff(); 
        Serial.println("RED  ");
        break;
      }
    }
  }
}

void blink(int globe, byte many){
  for (int i = 0; i < many; i++){
    digitalWrite(gleds[globe], HIGH);
    digitalWrite(rleds[globe], HIGH);  
    delay(1000);
    digitalWrite(gleds[globe], LOW);
    digitalWrite(rleds[globe], LOW);
    delay(1000);

  }
}
*/
void serialcomms() {

  if (Serial.available()){
    int command = Serial.read();
    switch (command){
    case '0':
      //	Serial.print("potDialVal    ");
      //	Serial.println(potDialVal);
      //Serial.print("potVal2     ");
      Serial.println("LOUGEDUINO TSMARTPad MOTOR CONTROLLER");
      Serial.print("       Time is   "); 
      Serial.println(time);
      Serial.print("Last  @   "); 
      Serial.println(lastup);
      Serial.print("Next in   "); 
      Serial.println(nextup);
      Serial.print("***Tilt timer @   "); 
      Serial.println(tilttimer); 
      Serial.print(" ****Win timer @   "); 
      Serial.println(wintimer);
      Serial.print("Current Average LUX is "); 
      Serial.println(aveLL.mean());
      delay(500);
      Serial.print("Current Average TempC is "); 
      Serial.println(aveLT.mean());
      readAndPrint();
  
      Serial.println(" 8- Lux Lumen Info");
      Serial.println(" 9-  SEND2SERVER ");
      delay(100);
      break;

    case '1':readAndPrint();break;
       /*   Serial.println(" Direction: FORWARD - KitchenTilt ... ");
      KitchenTilt->run(FORWARD);
      delay(tilttimer);
      KitchenTilt->run(RELEASE);
      delay(30); // 'coast' for 1/10 second
      readAndPrint();
      break;
    case 'z':      sendValueToLatch(1);break;      
    case 'x':      sendValueToLatch(2);      break;
    case 'c':      sendValueToLatch(4);break;    
    case 'v':     sendValueToLatch(8);      break;
	case 'm':      sendValueToLatch(0);      break;
    case 'q':
      Serial.println(" Direction: BACKWARD - KitchenTilt ... ");
      KitchenTilt->run(BACKWARD);
      delay(tilttimer);
      KitchenTilt->run(RELEASE); 
      readAndPrint();
      break;
    case '2':
      Serial.println(" WindowTILT ...");
      WindowTilt->run(FORWARD);
      delay(tilttimer); // 'coast' for 1/10 second
      WindowTilt->run(RELEASE);    
      readAndPrint();
      break;
    case 'w':
      Serial.println(" WindowTILT ...");
      WindowTilt->run(BACKWARD);
      delay(tilttimer); // 'coast' for 1/10 second
      WindowTilt->run(RELEASE);     
      readAndPrint();
      break;

    case '3':
      Serial.println(" DoorWindowTILT ...");
      DoorWindowTilt->run(FORWARD);
      delay(tilttimer); // 'coast' for 1/10 second
      DoorWindowTilt->run(RELEASE);     
      readAndPrint();
      break;
    case 'p':
  		Serial.println("ENTERING MANUAL CONTROL MODE, PRESS ANY KEY TO ESC");
       			while (command == 0){
       			int speed = map(potDialVal, 0, 1023, 0, 255);
       				if (potDialVal > 500) {
       					WindowTilt->setSpeed(speed);
       					WindowTilt->run(BACKWARD);
       					Serial.print("speed @ ");
       					Serial.println(speed);
       				}
       				else {
       					WindowTilt->setSpeed(speed);
       					WindowTilt->run(FORWARD);
       					Serial.print("speed @ ");
       					Serial.println(speed);
       				}
       		      Serial.print("done");
      break;

    case 'e':
      Serial.println(" DoorWindowTILTING BWD");
      DoorWindowTilt->run(BACKWARD);
      delay(tilttimer); // 'coast' for 1/10 second
      DoorWindowTilt->run(RELEASE);      
      break;

    case '4':
      Serial.println(" Tilting Lounge Blind");
      LoungeTilt->run(FORWARD);
      delay(tilttimer);
      LoungeTilt->run(RELEASE);        
      break;


    case 'r':
      Serial.println(" Tilting Lounge Blind");
      LoungeTilt->run(BACKWARD);
      delay(tilttimer);
      LoungeTilt->run(RELEASE);         
      break;

    case '5':
      Serial.println(" ALL FWD");            
      printSensors(); 
      WindowTilt->run(FORWARD);          
      KitchenTilt->run(FORWARD);        
      DoorWindowTilt->run(FORWARD);    
      LoungeTilt->run(FORWARD); 
      delay(tilttimer); 
      LoungeTilt->run(RELEASE);   
      DoorWindowTilt->run(RELEASE); 
      WindowTilt->run(RELEASE);  
      KitchenTilt->run(RELEASE); 
      break;
    case 't':           
      Serial.println(" ALL BWD");         
      printSensors();  
      WindowTilt->run(BACKWARD);        
      KitchenTilt->run(BACKWARD);        
      DoorWindowTilt->run(BACKWARD);    
      LoungeTilt->run(BACKWARD);
      delay(tilttimer); 
      LoungeTilt->run(RELEASE);  
      DoorWindowTilt->run(RELEASE);  
      WindowTilt->run(RELEASE);  
      KitchenTilt->run(RELEASE); 
      break;
    case '6':    
      kitchWinFwd();     
      printSensors();     
      break;
    case 'y': 
      kitchWinBwd();        
      printSensors();  
      break;

    case '7':  
      tilttimer = max(tilttimer - 1000, tiltmin); 
      Serial.println(""); 
      Serial.print("Tilt duration has been changed to "); 
      Serial.print(tilttimerS); 
      Serial.println("Seconds"); 
      Serial.print("...whilst WindowAction Duration now @"); 
      Serial.print(wintimerS); 
      Serial.println("Seconds"); 
      beep(tilttimerS);     
      readSensors(); 
      printSensors(); 
      break;
    case '8': 
      tilttimer = min(tilttimer + 1000, tiltmax); 
      Serial.println(""); 
      Serial.print("Tilt duration has been changed to "); 
      Serial.print(tilttimer / 1000); 
      Serial.println("Seconds"); 
      Serial.print("...whilst WindowAction Duration now @"); 
      Serial.print(wintimerS); 
      Serial.println("Seconds"); 
      beep(tilttimerS);     
      readSensors(); 
      printSensors(); 
      break;

    case 'u': 
      Serial.println("nada ");  
      break;
*/
    case '9':
      send2server();         
      break;
    }
  }
}

void setluxBar(){
  barlevel = map(lg_light, 0, 1023, 0, ledCount);
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    if (thisLed < barlevel) {
      digitalWrite(barpins[thisLed], HIGH);
    }
    else {            // turn off all pins higher than the ledLevel:
      digitalWrite(barpins[thisLed], LOW);
    }
  }
}

void sevSeg(int bitToSet) {
  clearsevSeg();
  if (bitToSet == 32) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, Tap[0]);
    digitalWrite(latchPin, HIGH);
  }
  //test for "."
  else if (bitToSet == 46) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, Tap[1]);
    digitalWrite(latchPin, HIGH);
  }
  //test for numbers
  else if (bitToSet <= 57) {
    bitToSet = bitToSet - 48;    
    digitalWrite(latchPin, LOW);    
    shiftOut(dataPin, clockPin, MSBFIRST, Tab[bitToSet]);    
    digitalWrite(latchPin, HIGH);
  }  
  else {	 //test for letters
    bitToSet = bitToSet - 97;    
    digitalWrite(latchPin, LOW);    
    shiftOut(dataPin, clockPin, MSBFIRST, Taf[bitToSet]);
    digitalWrite(latchPin, HIGH);
  }
}

void clearTTL(){
Serial2.write(0xFE); Serial2.write(0x58);delay(10);
}

void clearsevSeg(){		// attempt to clear the shift register 
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, Tab[0]);
  delay(100);	
  digitalWrite(latchPin, HIGH);
}

void kitttheBar(){
  int ledtimer = 30;
  for (byte count = 0; count < ledCount; count++) {     // cycles threw 
    digitalWrite(barpins[count], HIGH);      // turns the first led on
    delay(ledtimer);                             // sets the delay 
    digitalWrite(barpins[count + 1], HIGH);  // turns the next led on
    delay(ledtimer);                             // sets the delay 
    digitalWrite(barpins[count], LOW);       
    delay(ledtimer*1.5);                        
  }
  for (byte count = ledCount; count >= 0; count--) { 
    digitalWrite(barpins[count], HIGH);
    delay(ledtimer);
    digitalWrite(barpins[count- 1], HIGH);
    delay(ledtimer);
    digitalWrite(barpins[count], LOW);
    delay(ledtimer*1.5);
  }	
}

void readAndPrint(){
  readSensors();
  printSensors();
  setluxBar();
}
void readSensors() {
  lux = lightMeter.readLightLevel();
  lg_light = lux;
  lightPercent = map(lg_light, lightMin, lightMax, 0, 100);
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
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
    type_s = 1;    break;
  case 0x28:    //      Serial.println("  Chip = DS18B20");
    type_s = 0;    break;
  case 0x22:    //     Serial.println("  Chip = DS1822");
    type_s = 0;    break;
  default:    return;
  } 
  ds.reset();  ds.select(addr);  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  delay(1000);     // maybe 750ms is enough, maybe not
    present = ds.reset();  ds.select(addr);     ds.write(0xBE);         // Read Scratchpad
  //  Serial.print("  Data = ");
  //  Serial.print(present, HEX);
  //  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
     }
  int16_t raw = (data[1] << 8) | data[0];  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  }  else {
    byte cfg = (data[4] & 0x60);    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3;     else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  }
  celsius = (float)raw / 16.0;  // Serial.print("  Temperature = ");   Serial.print(celsius); Serial.print(" Celsius, ");
  temp_c = celsius;  dtostrf(temp_c,6, 2, tempbuffer);
  temperature = tempbuffer;  temperature.trim();
  aveLL.push(lg_light); 
  aveLT.push(temp_c); 
  logged++;
}
void printSensors(){
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println( "C ");
  sevSegPrint(temperature);
  Serial.print( "Light=");  
  Serial.print(lg_light);  
  Serial.print( "(lux)/" );  
  Serial.print(lightPercent);  
  Serial.println( "% " );
  Serial.println(" **  ");
} 
void printAverage(){
  readSensors();
  Serial.println(" Lux Stats");
  // And show some interesting results.
  Serial.print("Logged on"); 
  Serial.print(logged); 
  Serial.println("Occasions");
  Serial.print("Mean:   "); 
  Serial.println(aveLL.mean());
  Serial.print("Mode:   "); 
  Serial.println(aveLL.mode());
  //   Serial.print("Max:    "); Serial.println(aveLL.maximum(&maxat));
  //    Serial.print(" at:    "); Serial.println(maxat);
  //   Serial.print("Min:    "); Serial.println(aveLL.minimum(&minat));
  //  Serial.print(" at:    "); Serial.println(minat);
  //    Serial.print("StdDev: "); Serial.println(aveLL.stddev());
  Serial.println("Temp Stats");
  Serial.print("Mean:   "); 
  Serial.println(aveLT.mean());
  Serial.print("Mode:   "); 
  Serial.println(aveLT.mode());
  //   Serial.print("Max:    "); Serial.println(aveLT.maximum(&maxat));
  //    Serial.print(" at:    "); Serial.println(maxat);
  //   Serial.print("Min:    "); Serial.println(aveLT.minimum(&minat));
  //  Serial.print(" at:    "); Serial.println(minat);
  // Serial.print("StdDev: "); Serial.println(aveLT.stddev());
  delay(500);
}

void sevSegPrint(String Str){
  char temp[8];	
  Str.toCharArray(temp, 8);
  for (int i = 0; i < 8; i++) {
    sevSeg(temp[i]);
  }
  delay(100);
}

void beep(int num){
  for (int i = 0; i < num; i++){
    digitalWrite(piezoPin, HIGH);
    delay(500);
    digitalWrite(piezoPin, LOW);
    delay(500);
  }
}
/*
void kitchWinFwd(){
  Serial.print(" kitchWindow Opening "); 
  Serial.print(wintimerS); 
  Serial.println(" secs");  	  
  leds.setLEDs(LED_YELLOW);
  leds.update();
sendValueToLatch(1);
  delay(wintimer);
sendValueToLatch(0);
  delay(50);
}
void kitchWinBwd(){
  leds.setLEDs(LED_YELLOW);
  leds.update(); 
  Serial.println(" kitchWindow Closing "); 
  Serial.print(wintimerS); 
  Serial.println(" secs");
  delay(wintimer);	

}

*/
void send2server(){

  Serial.println("Time to send server");
  readAndPrint(); 
  //	aveLL.push(lg_light); aveLT.push(lg_temp); 

  String request = "GET " + repository + "sensor.php?lg_temp=" + aveLT.mean() +","+ aveLT.stddev() + "," + logged + " HTTP/1.0";
  String request2 = "GET " + repository + "sensor.php?lg_light=" + aveLL.mean() + "," + aveLL.stddev() + "," + logged + " HTTP/1.0";
  send_request(request);
  delay(20);	Serial.print("request: ");
  Serial.println(request);	
  Serial.println("Temp Data SENT");
  delay(20); 
  send_request(request2);
  Serial.print("request2: ");	
  Serial.println(request2);	
  Serial.println("Light Data SENT");
}
void send_request(String req) {
wdt_enable(WDTO_8S); wdt_reset();
  Serial.println("Attempting connection to server...");
 Adafruit_CC3000_Client client = cc3000.connectTCP(ip, port);
 delay(20);
 wdt_reset();
    if (client.connected()) {
    client.println(req);
    client.println(F(""));
	  }
  else {
    Serial.println(F("Connection failed"));
  }
  wdt_reset();
  while (client.connected()) {
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
  Serial.println("Connection Closed");
  Serial.println("");
  client.close();
wdt_reset();
wdt_disable();
}

void storeCode(void) {
  GotNew = true;
  codeType = My_Decoder.decode_type;
  if (codeType == UNKNOWN) {
    Serial.println("Received unknown code, ignoring...");
  }
  else {
    Serial.print(F("Received "));
    Serial.print(Pnames(codeType));
    if (My_Decoder.value == REPEAT) {
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

void knightrider() {
  int ledtimer;
  //	ledtimer = map(potDialVal, 0, 1023, ledmin, ledmax);   // maps the timer potValue to the max and min vaues set above with x and y
  for (byte count = 0; count < 8; count++) {     // cycles threw all the leds in the array
    digitalWrite(aleds[count], HIGH);      // turns the first led on
    delay(ledtimer);                             // sets the delay (for how fas it will go)
    digitalWrite(aleds[count + 1], HIGH);  // turns the next led on
    delay(ledtimer);                             // sets the delay (for how fas it will go)
    digitalWrite(aleds[count], LOW);       // turns it back off
    delay(ledtimer*1.5);                             // sets the delay (for how fas it will go)
  }
  for (byte count = 8; count >= 0; count--) {    
    digitalWrite(aleds[count], HIGH);
    delay(ledtimer);
    digitalWrite(aleds[count- 1], HIGH);
    delay(ledtimer);
    digitalWrite(aleds[count], LOW);
    delay(ledtimer*1.5);
  }
}
void redon() {
  for (byte count = 0; count > 4; count++) {    // same thing but in reverse so it will end up going back and forth
    digitalWrite(rleds[count], HIGH);
  }
}
void redoff() {
  for (byte count = 0; count > 4; count++) {    // same thing but in reverse so it will end up going back and forth
    digitalWrite(rleds[count], LOW);
  }
}
void gedon() {
  for (byte count = 0; count > 4; count++) {    // same thing but in reverse so it will end up going back and forth
    digitalWrite(gleds[count], HIGH);
  }
}
void gedoff() {
  for (byte count = 0; count > 4; count++) {    // same thing but in reverse so it will end up going back and forth
    digitalWrite(gleds[count], LOW);
  }
}

void settiltTime(){
  sevSegtime = time;
  sevSegtime = time;
  sevSegtimeS = (sevSegtime/1000);
  tilttimerS = (tilttimer/1000);
  wintimer = (tilttimer*5);
  wintimerS = (wintimer/1000);
}

void colorWipe(uint32_t c, uint8_t wait) {		// Fill the dots one after the other
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);    
    strip.show();    
    delay(wait);  
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();delay(wait);
  }
}

void theaterChase(uint32_t c, uint8_t wait) {		//Theatre-style crawling lights.
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void theaterChaseRainbow(uint8_t wait) {		//Theatre-style crawling lights with rainbow
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();      
      delay(wait);
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// The colours are a transition r - g - b - back to r. Input a value 0 to 255 to get a color value.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {    
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {    
    WheelPos -= 85;    
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;  
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


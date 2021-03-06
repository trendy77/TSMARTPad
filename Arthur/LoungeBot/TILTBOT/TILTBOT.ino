/*	MegaLoungeFallback
"The Graceful Retreat..."

TiltBot - Lounge
*/
bool readingtime = 0;
#include <IRLib.h>
#include <AFMotor.h>
#include <Wire.h>
#include <Servo.h> 
//as well as all loungebot libs for future upgrades/tinkering...
#include <Adafruit_CC3000.h>
#include <SPI.h>	// may not be possible??
#include <stdlib.h>
#include <OneWire.h>
#include <CC3000_MDNS.h>
#include <aREST.h>
#include <avr/wdt.h>
#include <BH1750.h>
#include <Average.h>
#include <LiquidTWI.h>
#include <FTRGBLED.h>
#include <Adafruit_NeoPixel.h>
<<<<<<< HEAD
#include <EEPROM.h>
=======

>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
#include <DFBLE.h>
// VARIABLES 	
// Sensors
OneWire ds(A1);  
BH1750 lightMeter;
IRrecv My_Receiver(A8);

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
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,SPI_CLOCK_DIV2);
uint32_t ip = cc3000.IP2U32(192,168,0,110);
int port = 80;
String repository = "/energy_project/";
unsigned long interval = 300000;           // every minute interval at which to send sensor data to server
unsigned long time;long nextup; long lastup;

#define NEOPIN A15
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, NEOPIN, NEO_GRB + NEO_KHZ800);

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
int tilttimer = 2000; 	// default length of "tilt fwd or bwd'
int tiltmax = 10000;
int tiltmin = 1000;
int testtimer = 1000; 
int sevSegtime = time;
int tilttimerS = (tilttimer / 1000);
int wintimer = (tilttimer * 5);
int wintimerS = (wintimer / 1000);

  Servo leftGear; Servo rightGear;  
  int pos; int invertpos;int move;   

<<<<<<< HEAD
AF_DCMotor kitchTilt(3, MOTOR12_64KHZ); 
AF_DCMotor winTilt(4, MOTOR12_64KHZ);
AF_DCMotor loungeTilt(1, MOTOR34_64KHZ);   
AF_DCMotor doorwTilt(2, MOTOR34_64KHZ);
=======
AF_DCMotor kitchTilt(3, MOTOR34_64KHZ); 
AF_DCMotor winTilt(4, MOTOR34_64KHZ);
AF_DCMotor loungeTilt(1, MOTOR12_64KHZ);   
AF_DCMotor doorwTilt(2, MOTOR12_64KHZ);
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef

const int piezoPin = A15;
int kw=31;
int kww=29;

int potVal =0;
IRTYPES codeType;      unsigned long codeValue;int codeBits; unsigned int rawCodes[RAWBUF];int rawCount; bool GotOne, GotNew;
IRdecode My_Decoder;
//int num;		// for BEEP function

int logged;int lg_light; int lg_temp;
const int lightMin = 0;const int lightMax = 1023;
int lightPercent;float celsius; float temp_c; 
String temperature; uint16_t lux;
Average<float> aveLL(10);
static char tempbuffer[10];
Average<float> aveLT(10);

byte Tab[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff}; //0,1,2,3,4,5,6,7,8,9, ALL OFF
byte Taf[] = {0xA0, 0x83, 0xa7, 0xa1, 0x86, 0x8e, 0xc2, 0x8b, 0xe6, 0xe1, 0x89, 0xc7, 0xaa, 0xc8, 0xa3, 0x8c, 0x98, 0xce, 0x9b, 0x87, 0xc1, 0xe3, 0xd5, 0xb6, 0x91, 0xb8};//a,b,c,d,e,f,g,h,i,j,k,l,o,m,n,o,p,q,r,s,t,u,v,w,x,y,z
byte Tap[] = {0xff, 0x7f}; //"space", "."
const int latchPin = A12;      //Pin connected to latch pin7SEG
const int clockPin = A13;
const int dataPin = A14;

#define PIN            24
#define NUMPIXELS      24

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup(){
  Serial.begin(115200);
  Serial.println("TLOUNGE INITIALISING...");

  strip.begin();  strip.setBrightness(255);	 strip.show(); 	
  colorWipe(strip.Color(255, 0, 0), 50);    // red?
  	   Serial2.begin(9600);
  // set the contrast-- 200 is a good middle place to try out
Serial2.write(0xFE);  Serial2.write(0x50);  Serial2.write(200);
  delay(10);       
    // set the brightness - we'll max it (255 is max brightness)
  Serial2.write(0xFE);  Serial2.write(0x99);  Serial2.write(255);
  delay(10);
  //backlight to Red
  Serial2.write(0xFE);Serial2.write(0xD0);Serial2.write(0x255);Serial2.write(0x0); Serial2.write(0x0);	
  delay(10);
<<<<<<< HEAD
  	
  Serial2.print("TrendySMARTPad -- Connecting");
  GotOne = false; GotNew = false;   codeType = UNKNOWN;  codeValue = 0;readingtime=false;
  kitchTilt.setSpeed(255);   kitchTilt.run(RELEASE);  doorwTilt.setSpeed(255);
  loungeTilt.setSpeed(255);  loungeTilt.run(RELEASE);winTilt.setSpeed(255);  winTilt.run(RELEASE);
doorwTilt.run(RELEASE);
 	pinMode(kw, OUTPUT); 	pinMode(kww, OUTPUT);

=======
  
<<<<<<< HEAD
strip.begin();  strip.setBrightness(255);colorWipe(strip.Color(255, 0, 0), 50);   	 strip.show(); 	
 // red?
=======
  strip.begin();  strip.setBrightness(255);	 strip.show(); 	
  colorWipe(strip.Color(255, 0, 0), 50);    // red?
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
  		
  Serial2.print("TrendySMARTPad -- Connecting");
  GotOne = false; GotNew = false;   codeType = UNKNOWN;  codeValue = 0;

<<<<<<< HEAD
  kitchTilt.setSpeed(255);  kitchTilt.run(RELEASE); doorwTilt.setSpeed(255);doorwTilt.run(RELEASE);
  loungeTilt.setSpeed(255);loungeTilt.run(RELEASE);  winTilt.setSpeed(255);  winTilt.run(RELEASE);
  
 	/*pinMode(potPin, INPUT);
=======
  kitchTilt.setSpeed(255);   doorwTilt.setSpeed(255);
  loungeTilt.setSpeed(255);  winTilt.setSpeed(255);  
  kitchTilt.run(RELEASE);doorwTilt.run(RELEASE);loungeTilt.run(RELEASE);winTilt.run(RELEASE);
 	pinMode(kw, OUTPUT); 	pinMode(kww, OUTPUT);
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
 leftGear.attach(10);  // attaches the servo on pin 9 to the servo object 
 rightGear.attach(9);  // attaches the servo on pin 9 to the servo object 
 pos = 0;            // start at Zeropoint allowing 180 deg movement 1 way...
 invertpos = 180;
 move = 1;           // servo moves this number of degrees each time left/right is pushed
 leftGear.write(pos); // Set initial position
 rightGear.write(invertpos); // Set initial position
>>>>>>> e172efcf5ce99788dea2af0383424f51ad5de190
 
	Serial1.begin(115200);	
	Serial1.println("hello on BT?");
	if (Serial1.available()){
   	Serial.println("Bluetooth Client Online");
   }   else {
   	Serial.println("BT Not Found");
   }
  //lightMeter.begin();
  //lux = lightMeter.readLightLevel();
  //lg_light = lux;

  Serial.println("initiALISING WiFi");
  rest.variable("lg_light", &lg_light);
  rest.variable("lg_temp", &lg_temp);
  rest.function("raiseKW",raiseKW);
  rest.function("lowerKW",lowerKW);
  rest.function("tiltFW",tiltFW);		// Function to be exposed
  rest.function("tiltBK",tiltBK);	
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
  wdt_reset();
  if (!mdns.begin("arduino", cc3000)) {   
    while(1);   
  }  
  restServer.begin();
  Serial.println("Connected to WiFi network");
*/ 
 Serial.println("TILTBOT SETUP COMPLETE");
 Serial.println("Press ZERO for Options");
Serial2.write(0xFE);Serial2.write(0xD0);Serial2.write(0x0);Serial2.write(0x0); Serial2.write(0x255);	// blue
delay(10);
clearTTL();
Serial2.write("CONNECTED!");
  My_Receiver.No_Output();//Turn off any unused IR LED output circuit
  My_Receiver.enableIRIn(); // Start the receiver
}

<<<<<<< HEAD
void loop(){
	while 		(GotOne==true){theaterChaseRainbow (20);}
=======
void tiltAllF(){
  Serial.println(" Direction: FORWARD - Tilting ALL... ");
  winTilt.run(FORWARD);
	  doorwTilt.run(FORWARD);
	  kitchTilt.run(FORWARD);
	  loungeTilt.run(FORWARD);
	  delay(1000);
	  winTilt.run(RELEASE);
      doorwTilt.run(RELEASE);
	  kitchTilt.run(RELEASE);
	  loungeTilt.run(RELEASE);
	  delay(50);
  }

<<<<<<< HEAD
void tiltAllB(){  Serial2.println(" Direction: BACKWARD - Tilting ALL... ");
=======
void tiltAllB(){  
Serial2.println(" Direction: BACKWARD - Tilting ALL... ");
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
  Serial.println(" Direction: BACKWARD - Tilting ALL... ");
  winTilt.run(BACKWARD);
	  doorwTilt.run(BACKWARD);
	  kitchTilt.run(BACKWARD);
	  loungeTilt.run(BACKWARD);
	  delay(1000);
	  winTilt.run(RELEASE);
      doorwTilt.run(RELEASE);
	  kitchTilt.run(RELEASE);
	  loungeTilt.run(RELEASE);
	  delay(50);
  delay(50);
   }
   
void clearTTL(){
Serial2.write(0xFE); Serial2.write(0x58);delay(10);
}
    /*   
void windowSense(){
	digitalWrite(trigPin, LOW);  // Added this line
	delayMicroseconds(2); // Added this line
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10); // Added this line
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	distance = (duration / 2) / 29.1;

	if (distance >= 3) {  // This is where the LED On/Off happens
		loungeWinstate = 1;
	}
	else {
		loungeWinstate = 0;
	}

	Serial.print(distance);
	Serial.println("cm");

	
	delay(200);
}
*/


void settiltTime(){
  sevSegtime = time;
  sevSegtimeS = (sevSegtime/1000);
  tilttimerS = (tilttimer/1000);
  wintimer = (tilttimer*5);
  wintimerS = (wintimer/1000);
}


void loop(){
<<<<<<< HEAD
=======
>>>>>>> e172efcf5ce99788dea2af0383424f51ad5de190
  clearTTL();
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
 time = millis();
  nextup = ((interval + lastup) - time);
  Serial2.print("time is ");
   Serial2.print(time); 
	//leftGear.write(invertpos); rightGear.write(pos);

	if (My_Receiver.GetResults(&My_Decoder)) {
		GotOne=true;
		My_Decoder.decode();
		delay(100);
			if(My_Decoder.decode_type==MY_PROTOCOL) {
			Serial.print("COMMAND RECEIVED");
			IRrec();	
		

        }		My_Receiver.resume();
      }
serialcomms();
<<<<<<< HEAD
Serial.println(".");
=======
Serial.println(".");delay(500);
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
}

void clearTTL(){
Serial2.write(0xFE); Serial2.write(0x58);delay(10);
}

void lTilt(){
	  Serial.println(" Direction: FORWARD - Tilting l... ");
  loungeTilt.run(FORWARD);	  delay(1000);	  loungeTilt.run(RELEASE);	  delay(50);
}
void lTiltR(){
	  Serial.println(" Direction: BACKWARD - Tilting l... ");
  loungeTilt.run(BACKWARD);	  delay(1000);	  loungeTilt.run(RELEASE);	  delay(50);
}
void kTilt(){
	   Serial.println(" Direction: FORWARD - Tilting k... ");
  kitchTilt.run(FORWARD);	  delay(1000);	  kitchTilt.run(RELEASE);	  delay(50);
	  }
void kTiltR(){
	  Serial.println(" Direction: BACKWARD - Tilting k... ");
 kitchTilt.run(BACKWARD);	  delay(1000);	  kitchTilt.run(RELEASE);	  delay(50);
}
void dwTilt(){
	  Serial.println(" Direction: FORWARD - Tilting dw... ");
  doorwTilt.run(FORWARD);	  delay(1000);	  doorwTilt.run(RELEASE);      delay(50);
}
void dwTiltR(){
	  Serial.println(" Direction: BACKWARD - Tilting dw... ");
  doorwTilt.run(BACKWARD);	  delay(1000);	  doorwTilt.run(RELEASE);      delay(50);
}
void wTilt(){
	   Serial.println(" Direction: FORWARD - Tilting win... ");
  winTilt.run(FORWARD);  delay(1000);	  winTilt.run(RELEASE);      delay(50);
}
void wTiltR(){
	  Serial.println(" Direction: BACKWARD - Tilting win... ");
  winTilt.run(BACKWARD); 	  delay(1000);	  winTilt.run(RELEASE);      delay(50);
}
void IRrec(){
   switch(My_Decoder.value) {
      //      case LEFT_ARROW:   tiltAllF();break;
  //          case RIGHT_ARROW:   tiltAllB(); break;
			case SELECT_BUTTON: pos=90; break;
            case DOWN_ARROW: tilttimer = min(tilttimer + 1000, tiltmax);       Serial.println("");       Serial.print("Tilt duration has been changed to "); 
      Serial.print(tilttimer / 1000);       Serial.println("Seconds");       Serial.print("...whilst WindowAction Duration now @");       Serial.print(wintimerS);       Serial.println("Seconds");       beep(tilttimerS);break;
<<<<<<< HEAD
           			case BUTTON_1:		kTilt();break;
=======
            move=max(1, move-1); break;
			case BUTTON_1:		kTilt();break;
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
			case BUTTON_2:		kTiltR();break;
			case BUTTON_3:		wTilt();break;
			case BUTTON_4:		wTiltR();break;
			case BUTTON_5:		dwTilt();break;
			case BUTTON_6:		dwTiltR();break;
			case BUTTON_7:		lTilt();break;
			case BUTTON_8:		lTiltR();break;
 //  case PRESET_PREV: OpensenseMoveWin();break;
    case PRESET_NEXT: senseMoveWin(); break;
	//	case BLUE:   pos=min(180,pos+move);  break;
      //  case YELLOW: pos=max(0,pos-move); break; 
		}
<<<<<<< HEAD
	//	invertpos = (pos -180);
//leftGear.write(pos); rightGear.write(invertpos);
=======

>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
   }  
 
void senseMoveWin(){
}
void serialcomms(){
<<<<<<< HEAD
	if (Serial.available()){
=======
	Serial2.write(0xFE); Serial2.write(0xD0); Serial2.write(0x255); Serial2.write(0x255); Serial2.write(0x255);	// White
delay(20);
if (Serial.available()){
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
		int command = Serial.read();
		switch (command){
			case '0':
      // windowSense();
	  Serial.println("TSMARTPad TILT CONTROLLER");
      Serial.print("Time is   "); 
      Serial.println(time);
      Serial.print("Last  @   "); 
      Serial.println(lastup);
      Serial.print("Next in   "); 
      Serial.println(nextup);
      Serial.print("***Tilt timer @   "); 
      Serial.println(tilttimer); Serial.println("TILTBOT OPTIONS");
	Serial.println("ir - ");
  //Serial.print("distance @ ");
	//Serial.println(distance);
	  Serial.println(" 1/q-  KitchenTILT 5 SEC ");
      Serial.println(" 2/w-  WindowTILT 5 SEC ");
      Serial.println(" 3/e- DoorWindowTILT 5 SEC ");
      Serial.println(" 4/r- LoungeTilt 5 SEC ");
      Serial.println(" **5/t- ALL TILT 6 SEC");
      Serial.println(" 6/y- KITCHEN WINDOW OPEN/CLOSE");
      Serial.println("   P = POTMODE ALL TILTSPEED ** ");
      Serial.println(" z/x/c/v- relays 1-4; m to reset ");
      Serial.println(" 7/8- + OR - TILTIMER");
      Serial.println(" 9-  SEND2SERVER ");
      delay(100);
      break;
	  	case '1':		kTilt();break;
			case 'q':		kTiltR();break;
			case '2':		wTilt();break;
			case 'w':		wTiltR();break;
			case '3':		dwTilt();break;
			case 'e':		dwTiltR();break;
			case '4':		lTilt();break;
			case 'r':		lTiltR();break;
	 // case '5':	  tiltAllF(); break;
	//  case 't':  tiltAllB();break;
	   case '9':  
      tilttimer = max(tilttimer - 1000, tiltmin); 
        Serial.print("Tilt duration has been changed to "); Serial.print(tilttimerS); Serial.println("Seconds"); 
      Serial.print("...whilst WindowAction Duration now @"); Serial.print(wintimerS); Serial.println("Seconds"); 
      beep(tilttimerS); break;
    case '8': 
      tilttimer = min(tilttimer + 1000, tiltmax); Serial.println(""); Serial.print("Tilt duration has been changed to "); Serial.print(tilttimer / 1000); Serial.println("Seconds"); Serial.print("...whilst WindowAction Duration now @"); Serial.print(wintimerS); Serial.println("Seconds"); 
      beep(tilttimerS); break;
/*    case '1':
      Serial.println(" Direction: FORWARD - KitchenTilt ... ");
      KitchenTilt->run(FORWARD);
      delay(1000);
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
      delay(1000);
      KitchenTilt->run(RELEASE); 
      readAndPrint();
      break;
		case '2':
		delay(100);
		break;
      
		case '1':senseMoveWin();
		break;
		case '2': closelwin();
         break;
        
	case '3': openlwin();
	break;
        
	case '4':
    lowerblind();
	break;

	
      */  
	case '6':
digitalWrite(kw, HIGH);        digitalWrite(kww, LOW);delay(2000);  digitalWrite(kw, LOW);
	break;

	case '7':
digitalWrite(kww, HIGH);       digitalWrite(kw, LOW); delay(2000);  digitalWrite(kww, LOW);
	break;

		}
	}
}
/*
void closeWin() {
 //  windowSense2();
//   if (distance2 > 3 ) {
 Serial.println("window CLOSE");
digitalWrite(kww, HIGH);      
 // while (distance2 > 3) {
   //   windowSense2();
    //  Serial.print("dist @"); Serial.print(distance2);
    }
digitalWrite(kw, LOW);      
    delay(500);
    Serial.println("configuring final height");
  //  windowSense2();
//  shortWinC();
 // if (distance2 >= 1){
  shortWinC();
  }
//}
}
void openWin() {
  Serial.println("window OPEN");
   if (distance2 <15 ) {
 Serial.println("window CLOSE");
  sendValueToLatch(32);
  while (distance2 < 20) {
      windowSense2();
      Serial.print("dist @"); Serial.print(distance2);
    }
    sendValueToLatch(0);
    delay(500);
  }
}	

void shortWinO() {
  sendValueToLatch(32);
  Serial.println("miniOpening ");
  delay(1500);
  sendValueToLatch(0);
}
void shortWinC() {
  sendValueToLatch(4);
  Serial.println("miniOpening ");
  delay(1500);
  sendValueToLatch(0);
}
*/
void storeCode(void) {
  GotNew=true;
  codeType = My_Decoder.decode_type;
  if (codeType == UNKNOWN) {
   }
  else {
     if (My_Decoder.value == REPEAT) {
        Serial.println(F("repeat; ignoring."));
     }
     else {
       codeValue = My_Decoder.value;       codeBits = My_Decoder.bits;
     }
      }
}
<<<<<<< HEAD
=======

>>>>>>> e172efcf5ce99788dea2af0383424f51ad5de190
void readAndPrint(){
  readingtime = TRUE;
  while (readingtime == TRUE){
	  rainbow (10);
 readSensors();
  printSensors();
  //setluxBar();
  }
}
void readSensors() {
  
}
void printSensors(){
  } 
void printAverage(){
  delay(500);
}

void beep(int num){
  for (int i = 0; i < num; i++){
    digitalWrite(piezoPin, HIGH);
    delay(500);
    digitalWrite(piezoPin, LOW);
    delay(500);
  }
}
<<<<<<< HEAD

=======
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
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
<<<<<<< HEAD
=======

<<<<<<< HEAD
=======
>>>>>>> e172efcf5ce99788dea2af0383424f51ad5de190
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

>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef
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
<<<<<<< HEAD

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

=======
>>>>>>> b9a19a7aace5f282f7418d1172a4f38ae3270fef

#include "Keypad.h"	 // keypad type definition
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <SPI.h>
//#include <SoftwareSerial.h>
#include <DHT.h>

Adafruit_MotorShield AFMSbot = Adafruit_MotorShield(); 
	Adafruit_StepperMotor *LoungeStep = AFMSbot.getStepper(200, 1);
    Adafruit_DCMotor *LoungeTest = AFMSbot.getMotor(3);
	Adafruit_DCMotor *LoungeTest2 = AFMSbot.getMotor(4);

Adafruit_MotorShield AFMSi = Adafruit_MotorShield(0x61);
  Adafruit_DCMotor *KitchenTilt = AFMSi.getMotor(3);
  Adafruit_DCMotor *WindowTilt = AFMSi.getMotor(4);
  Adafruit_DCMotor *DoorWindowTilt = AFMSi.getMotor(2);
  Adafruit_DCMotor *LoungeTilt = AFMSi.getMotor(1);

//DHT11 sensor pins
#define DHTPIN 49
#define DHTTYPE DHT22

  //KEYPAD
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] =
{{'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}};
byte rowPins[ROWS] = {30, 28, 26, 24}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {38, 36, 34, 32}; // connect to the column pinouts of the keypad
int keycount=0;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

  // K.I.T.T. LIGHTING
		const int led1 = 23;
			bool led1state;		// ** ADD IN FOR EACH LED? THEN SAVE STATE OF LED TO THIS ie 		
			const int led2 = 25;
       	const int led3 = 27;
        const int led4 = 29;
	const int ledpinArray[] = {led1, led2, led3, led4};  // lists the pins the leds are attached to
		const int red1 = 31;
        const int red2 = 33;
        const int red3 = 35;
        const int red4 = 37;
	int redledpinArray[] = {red1, red2, red3, red4};  // lists the pins 
			byte count = 0;     // used so the	 program knows where it is in the sequence
			int ledmin = 80;        // sets the max speed (0 = fast) the lower the number the faster it can go
			int ledmax = 200;      // sets the min speed (100 = slow) the higher the number the slower it can go
// LIGHT & TEMP
#define REF_PIN 15
	void getCurrentTemp( int *sign, int *whole, int *fract);
	char temp_string[10];
const int lread = A0;
	int LightSensor11 = 0;
		const int lsensorMin = 0;      // sensor minimum, discovered through experiment
		const int lsensorMax = 1023;    // sensor maximum, discovered through experime
DHT dht(DHTPIN, DHTTYPE);
int temperature;
int lightRead;
int humidity;

//SoftwareSerial bttSerial(15, 14); // RX, TX
/*
#include <Servo.h>
	Servo servo1;
	Servo servo2;
		int servopos = 0; //servo pos
		int cuntServopos = 0;    // variable to store the servo position
#include <FTRGBLED.h>
	  const int NUM_LEDS = 2;
	  const int PIN_CKI = 7;
	  const int PIN_SDI = 6;
	  RGBLEDChain leds(NUM_LEDS, PIN_CKI, PIN_SDI);
  */

int buttonPin = 43;
		int buttonState = 0;
int potPin = 41;
		int potval = 0;
			int prevPot = 0;
int move77 = 0;

// ** OPEN SERIAL1 (TX - 18, RX - 19) FOR COMMS WITH ANOTHER ARDUINO
int val = 0;
int incomingUSB = 0;
int incomingS1 = 0;
//** OPEN SERIAL3 TX RX FOR BT COMMS 
int incomingBT = 0; 

void setup() {
 
AFMSi.begin();		  // Start the underlying shields
		delay(50);
/*AFMSbot.begin();
        delay(50);
  */
    Serial.begin(115200);
  Serial.println("TLOUNGE INITIALISING...");
  //Start your Engines...
 LoungeTilt->setSpeed(255);
  LoungeTilt->run(RELEASE);
  KitchenTilt->setSpeed(255);
  KitchenTilt->run(RELEASE);
  DoorWindowTilt->setSpeed(255);
  DoorWindowTilt->run(RELEASE);
  WindowTilt->setSpeed(255);
  WindowTilt->run(RELEASE);
  	delay(50);
		// Top 2nd Shield 
//	LoungeStep->setSpeed(20.0);
	//LoungeTest->setSpeed(255);
 //LoungeTest->run(RELEASE);
 //LoungeTest2->setSpeed(255);
 //LoungeTest2->run(RELEASE);
  //delay(100);

  
  Serial1.begin(115200); //Begin Serial to talk to the Rx Arduino
  Serial.println("Serial1 Connection OPEN"); 
  Serial.parseInt();     //clear any garbage in the buffer.  
  //bttSerial.begin();
  //bttSerial.println("what's that saying about expecting a different result?");
  
  dht.begin();
  
  keypad.addEventListener(keypadEvent);  // Add an event listener.
  keypad.setHoldTime(1500);               // Default is 1000mS
  keypad.setDebounceTime(40);           // Default is 50mS

  if (Serial3.available())//S3 be7bf gluetoothbhopedulky...
  {    int command = 0;		// for serial commands
command = (Serial3.read());
Serial.write(command);
  }
  if (Serial.available()){
	  int command = 0;		// for serial commands
    command = (Serial.read());
	Serial3.write(command);
  }
// FOR MASTER MEGA, KITT LEDS TO BE CONTROLLED BY UNO VIA SERIAL COMMS?
  Serial.println("K.I.T.T. INITIALIZING...");
	for (count = 0; count <= 4; count++) { 
		pinMode(ledpinArray[count], OUTPUT);      // sets all the pins in the array as outputs
		pinMode(redledpinArray[count], OUTPUT);      
		}
		delay(50);
}

/*KEYPAD CONTROLS
 'PRESS' FOR STD 5-7SEC TILT, 
 'hold' to tilt until button depressed
 TOP ROW FWD  nos. 1-3  are tilt'
2nd row BWD tilt nos 1-3,-----> A & B are all in!
 3rd - fwd loungetilt and stepper fwd
4th - step kitchstep bwd; step loungestep FWD BKD
*/

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    
	case PRESSED:
      if (key == 'A') {
        Serial.println("A pressed, Tiltiing FWD ALL IN");
        LoungeTilt->run(FORWARD);
		KitchenTilt->run(FORWARD);
        WindowTilt->run(FORWARD);
        DoorWindowTilt->run(FORWARD);
        delay(5000);
        KitchenTilt->run(RELEASE);
		WindowTilt->run(RELEASE);
        DoorWindowTilt->run(RELEASE);
        LoungeTilt->run(RELEASE);
		readsensors();
		fetchsensors();		// 'FETCH'  from roomduino/webduino/etc?
	  }
    	break;
      if (key == 'B') {
        Serial.println("A pressed, Tiltiing FWD ALL IN");
        LoungeTilt->run(BACKWARD);
		KitchenTilt->run(BACKWARD);
        WindowTilt->run(BACKWARD);
        DoorWindowTilt->run(BACKWARD);
        delay(5000);
        KitchenTilt->run(RELEASE);
		WindowTilt->run(RELEASE);
        DoorWindowTilt->run(RELEASE);
        LoungeTilt->run(RELEASE);
		readsensors();
		fetchsensors();		// 'FETCH'  from roomduino/webduino/etc?
	  }
    	break;
      if (key == 'C') {
	  Serial.println("C pressed, STEPPING WINDOWS FWD ALL IN");
       LoungeStep->step(2000, FORWARD, DOUBLE);
        delay (7500);
       readsensors();
		fetchsensors();		// 'FETCH'  from roomduino/webduino/etc?
      }
      break;
            if (key == 'D') {
		Serial.println("C pressed, STEPPING WINDOWS BKD ALL IN");
       LoungeStep->step(2000, BACKWARD, DOUBLE);
        delay (7500);
       readsensors();
		fetchsensors();		// 'FETCH'  from roomduino/webduino/etc?
		      }
      break;
	  if (key == '1') {
        KitchenTilt->run(FORWARD);
        delay(5000);
        KitchenTilt->run(RELEASE);
        Serial.println("#1 pressed, KitchenTilt Activated");
      }
      break;
      if (key == '2') {
                Serial.println(" #2 pressed; WindowTILT ... FWD");
        WindowTilt->run(FORWARD);
        delay (5000);
        WindowTilt->run(RELEASE);
        delay(30); // 'coast' for 1/10 second
Serial.println("#2 pressed, WindowTilt Activated");
      }
      break;
if (key == '3') {
        DoorWindowTilt->run(FORWARD);
        Serial.println("#3 pressed, DoorWindowTilt Activated");
		delay(5000);
        DoorWindowTilt->run(RELEASE);
              }
      break;  
      if (key == '4') {
        KitchenTilt->run(BACKWARD);
        delay(5000);
        KitchenTilt->run(RELEASE);
        Serial.println("#4 pressed, KitchenTilt BKD Activated");
      }
      break;
      if (key == '5') {
        WindowTilt->run(BACKWARD);
        Serial.println("#5 pressed, WindowTilt BKD Activated");
		delay(5000);
        WindowTilt->run(RELEASE);
        }
      break;
      if (key == '6') {
        DoorWindowTilt->run(BACKWARD);
        delay(5000);
        DoorWindowTilt->run(RELEASE);
        Serial.println("#6 pressed, DoorWindowTilt BKD Activated");
      }
      break;
      if (key == '7') {
        Serial.println("#7 pressed, LioungeTilt FWD");
        LoungeTilt->run(FORWARD);
		delay(5500);
		LoungeTilt->run(RELEASE);
			 }
	   break;
      if (key == '8') {
        Serial.println("#8 pressed, LoungeTilt BKD");
        LoungeTilt->run(BACKWARD);
		delay(5500);
		LoungeTilt->run(RELEASE);
		}
	  break;
      if (key == '9') {
        Serial.println("#9 presssed?");
      readsensors();
	  }
        break;
      if (key == '*') {
        Serial.println(" * pressed, lights off");
        knightrider();
		lightsoff();
		}
		break;
            if (key == '0') {
        Serial.println("#0 pressed, blinking lights");
		blinklights();
		}
        break;
      if (key == '#') {
        Serial.println("# pressed, lights on");
      lightson();
	  }
      break;
     
	 case HOLD:
	 if (key == 'A') {
        Serial.println("A CURRENTLY pressed, Tiltiing FWD ALL IN");
        LoungeTilt->run(FORWARD);
		KitchenTilt->run(FORWARD);
        WindowTilt->run(FORWARD);
        DoorWindowTilt->run(FORWARD);
      	break;
      }
      if (key == 'B') {
		     Serial.println("B CURRENTLY pressed, Tiltiing BKD ALL IN");
        LoungeTilt->run(BACKWARD);
		KitchenTilt->run(BACKWARD);
        WindowTilt->run(BACKWARD);
        DoorWindowTilt->run(BACKWARD);
      }
	  break;
      if (key == 'C') {
	  Serial.println("C pressed, STEPPING WINDOWS FWD ALL IN");
       LoungeStep->step(5, FORWARD, DOUBLE);
      }
      break;
            if (key == 'D') {
		Serial.println("C pressed, STEPPING WINDOWS BKD ALL IN");
       LoungeStep->step(5, BACKWARD, DOUBLE);
		      }
      break;
	  if (key == '1') {
        KitchenTilt->run(FORWARD);
        Serial.println("#1 BEING pressed, KitchenTilt Activated");
      }
      break;
      if (key == '2') {
                Serial.println(" WindowTILT ... 5sec @ 255 BKD");
        WindowTilt->run(FORWARD);
Serial.println("#2 BEING pressed, WindowTilt Activated");
      }
      break;
if (key == '3') {
        DoorWindowTilt->run(FORWARD);
        Serial.println("#3 DEpressed, DoorWindowTilt Activated");
      }
      break;  
      if (key == '4') {
        KitchenTilt->run(BACKWARD);
        Serial.println("#4 DEpressed, KitchenTilt BKD Activated");
      }
      break;
        WindowTilt->run(BACKWARD);
      if (key == '5') {
        Serial.println("#5 DEpressed, WindowTilt BKD Activated");
		}
      break;
      if (key == '6') {
        DoorWindowTilt->run(BACKWARD);
        Serial.println("#6 DEpressed, DoorWindowTilt BKD Activated");
      }
      break;
      if (key == '7') {
		Serial.println("#7 DEpressed, lLoungeTilt Activated:");
 LoungeTilt->run(FORWARD);
              }
	   break;
      if (key == '8') {
        Serial.println("#8 DEpressed, LoungeTilt BKD");
     LoungeTilt->run(BACKWARD);
	 }
	  break;
      if (key == '9') {
        Serial.println("#9BEING PRESSED - NADA FOR NOW");
      }
        break;
      if (key == '*') {
		}
		break;
            if (key == '0') {
		}
        break;
      if (key == '#') {
	  }
      break; 

	  case RELEASED:
      if (key == 'A') {
        LoungeTilt->run(RELEASE);
        KitchenTilt->run(RELEASE);
        DoorWindowTilt->run(RELEASE);
        LoungeTilt->run(RELEASE);
        Serial.println("A released, motors stopped");
      }
      break;
      if (key == 'B') {
        LoungeTilt->run(RELEASE);
        KitchenTilt->run(RELEASE);
        DoorWindowTilt->run(RELEASE);
        LoungeTilt->run(RELEASE);
        Serial.println("B released");
      }
      break;
      if (key == 'C') {
// NO WAY TO RELEASE STEPPER 
        Serial.println("C button UNpressed - cut the motors!");
        KitchenTilt->run(RELEASE);
        DoorWindowTilt->run(RELEASE);
        LoungeTilt->run(RELEASE);
		}
      break;
	  if (key == '1') {
        KitchenTilt->run(RELEASE);
        Serial.println("button UNpressed - cut the motors!");
      }
      break;
      if (key == '2') {
                Serial.println(" WindowTILT ... 5sec @ 255 BKD");
        WindowTilt->run(RELEASE);
Serial.println("button UNpressed - cut the motors!");
      }
      break;
if (key == '3') {
        DoorWindowTilt->run(RELEASE);
        Serial.println("button UNpressed - cut the motors!");
      }
      break;  
      if (key == '4') {
        KitchenTilt->run(RELEASE);
        Serial.println("button UNpressed - cut the motors!");
      }
      break;
  }
}


void loop() {   
char key = keypad.getKey();

  if (key) {
      Serial.println(key);
   //   From the PC/USB
/*  incomingUSB = Serial.available();  
  while (incomingUSB != 0)           //while there is something to be read
  {
    val = Serial.parseInt();
    if (val != 0)
    {
      Serial.println(val);         //Print value to the Serial Monitor
      Serial1.print(val);          //Send value to the SLAVE Arduino
    Serial3.print(val);
	}
    incomingUSB = Serial.available();
  }
 //     From the SLAVE Arduino
  incomingS1 = Serial1.available();  
  while(incomingS1 != 0)           //While there is something to be read
  {
    val = Serial1.parseInt();    //Get new value
    Serial.print("Receiving...  ");    
    Serial.println(val);        //Print new value to the Serial Monitor
    incomingS1 = Serial1.available();
  }
    incomingBT = Serial3.available();  
  while(incomingBT != 0)           //While there is something to be read
  {
    val = Serial3.parseInt();    //Get new value
    Serial.print("Receiving via BT...  ");    
    Serial.println(val);        //Print ndex value to the Serial Monitor
    incomingBT = Serial3.available();
  }
  */
  }
  buttonState = (analogRead(buttonPin));
potval = analogRead(potPin);

Serial.println("READY - Press ZERO for Options");
Serial.print("pot is @");
Serial.println(potval);
Serial.print("ButtonState @");
Serial.println(buttonState);
	
/*}
 if (buttonState <100){
serialcommsFWD();
}
else if ((buttonState >580) && (buttonState <620)){
	*/
	serialcomms();
/*	}
else {
  Serial.println("button config error - PRESS '1' FOR FORWARD, '2' FOR BACKWARD");
int command;
command = Serial.read();
    switch (command)
    {
      case '1':
	  serialcommsFWD();
	  break;
	  
	  case '2':
	  serialcommsBKD();
	  break;
  }
  }
*/
 prevPot = potval;
}

void buttoncheck(){
	buttonState = (analogRead(buttonPin));
	  if ((buttonState > 665) && (buttonState < 675)) {
    Serial.println("LoungeTILT button pressed!");
  LoungeTilt->run(BACKWARD);
    Serial.println("Tilting BKD");
delay(1000);
  LoungeTilt->run(RELEASE);
			} else if ((buttonState > 205) && (buttonState < 212)) {
    Serial.println("WindowTILT button pressed!");
    Serial.println("Tilting FWD");
	WindowTilt->run(FORWARD) ;
	delay(1000);
	WindowTilt->run(RELEASE);
Serial.println("Finished");
			} else if ((buttonState > 635) && (buttonState < 640)) {
    Serial.println("WindowTILT button pressed!");
    Serial.println("Tilting BKD");
  WindowTilt->run(BACKWARD);
  delay(1000);
  WindowTilt->run(RELEASE);
Serial.println("Finished");
  } else if ((buttonState > 830) && (buttonState < 838)) {
    Serial.println("DoorWindowTILT button pressed!!");
    Serial.println("Tilting FWD");
DoorWindowTilt->run(FORWARD);
delay(1000);
DoorWindowTilt->run(RELEASE);
Serial.println("Finished");
  } else if ((buttonState > 870) && (buttonState < 877)) {
    Serial.println("DoorWindowTILT button pressed!");
    Serial.println("Tilting BKD");
	DoorWindowTilt->run(BACKWARD);
delay(1000);
DoorWindowTilt->run(RELEASE);
Serial.println("Finished");
}  else if ((buttonState > 812) && (buttonState < 817)) {
    Serial.println("KitchenTILT button pressed!");
    Serial.println("Tilting FWD");
	KitchenTilt->run(FORWARD);
	delay(1000);
  KitchenTilt->run(RELEASE);
Serial.println("Finished");
  }		else if ((buttonState > 733) && (buttonState < 740)) {
    Serial.println("KitchenTILT button pressed!");
    Serial.println("Tilting BKD");
  KitchenTilt->run(BACKWARD);
  delay(1000);
  KitchenTilt->run(RELEASE);
Serial.println("Finished");
  }
  }

void serialcomms() {
if (Serial.available())
  {
int command;
command = Serial.read();
    switch (command)
    {
      case '0':
        Serial.println("button is ");
		Serial.println(buttonState);
 	    Serial.println("LOUGEDUINO TSMARTPAD MOTOR CONTROLLER");
		knightrider();
		readSensors();
        Serial.println(" 1-  KitchenTILT 5 SEC ");
        Serial.println(" 2-  WindowTILT 5 SEC ");
        Serial.println("                      ");
        Serial.println(" 3- DoorWindowTILT 5 SEC ");
        Serial.println(" 4- LoungeTILT 5 SEC ");
        Serial.println("                      ");
        Serial.println(" 5- LoungeStep 1 REV");
        Serial.println(" 6- LoungeStep 10 REVS");
        Serial.println("                      ");
        Serial.println(" 7- LoungeTest 5 SECS ");
        Serial.println(" 8- PotpinREAD");
        Serial.println("                      ");
        Serial.println("9-ALL IN MOFO!... ");
        delay(100);
        break;

      case '1':
        Serial.println(" Direction: BACKWARDS - KitchenTilt ... 5sec ");
        KitchenTilt->run(BACKWARD);
        delay(5000);
        KitchenTilt->run(RELEASE);
        delay(30); // 'coast' for 1/10 second
        readSensors();
        break;
        //Serial.println(" Direction: BACKWARD - KitchenTilt ... 5sec ");
        //delay(3000);                        
        //KitchenTilt->run(BACKWARD);    

      case '2':
        Serial.println(" WindowTILT ... 5sec @ 255 BKD");
        WindowTilt->run(BACKWARD);
        delay (5000);
        WindowTilt->run(RELEASE);
        delay(30); // 'coast' for 1/10 second
        readSensors();
		break;

      case '3':
        Serial.println(" DoorWindowTILT ... 5sec @ 255 BKD");
        DoorWindowTilt->run(BACKWARD);
        delay (5000);
        DoorWindowTilt->run(RELEASE);
        delay(30); // 'coast' for 1/10 second
        break;

      case '4':
        Serial.println(" LoungeTilt ... 5sec @ 255 BKD");
        LoungeTilt->run(BACKWARD);
        delay (5000);
        LoungeTilt->run(RELEASE);
        delay(30); // 'coast' for 1/10 second
        break;

      case '5':
        Serial.println(" LoungeStep ... 1 Rev BWD");
        LoungeStep->step(200, BACKWARD, DOUBLE);
        delay(30); // 'coast' for 1/10 second
        readSensors();
        break;

      case '6':
        Serial.println(" LoungeStep 10 REVS");
        LoungeStep->step(2000, BACKWARD, DOUBLE);
        delay (5000);
        readSensors();
		break;

      case '7':
        Serial.println(" LoungeSTest2");
        LoungeTest->run(BACKWARD);
        LoungeTest2->run(BACKWARD);
        delay(5000); // 'coast' for 1/10 second
        LoungeTest->run(RELEASE);
        LoungeTest2->run(RELEASE);
		        delay(30); // 'coast' for 1/10 second
				readSensors();
        break;

      case '8':
	move77 = (potval - prevPot);
Serial.println("pot pin is @");
Serial.println(move77);
	 delay(30); 
// 'coast' for 1/10 second
	break;
case '9':
	  		  Serial.println("All in REVERSE??...");
        LoungeTilt->run(BACKWARD);
        KitchenTilt->run(BACKWARD);
        WindowTilt->run(BACKWARD);
        DoorWindowTilt->run(BACKWARD);
        delay(5000);
        KitchenTilt->run(RELEASE);
        DoorWindowTilt->run(RELEASE);
        LoungeTilt->run(RELEASE);
        WindowTilt->run(RELEASE);
	delay(30);
	}
    }
}

void readSensors() {
	getCurrentTemp(temp_string);
  int LightPercent;
  LightSensor11 = analogRead(lread);
  LightPercent = map(LightSensor11, lsensorMin, lsensorMax, 0, 100);
  Serial.print( "Temp =   ");
  Serial.print(temp_string);
  Serial.println( "C ");
  Serial.print( "Light =" );
  Serial.println(LightSensor11);
   temperature = (uint8_t)dht.readTemperature();
  humidity = (uint8_t)dht.readHumidity();
  Serial.print("Humidity: "); 
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(temperature);
  Serial.print(" *C ");
}

	void knightrider() {
int ledtimer;
	int timer;
			timer = analogRead(potPin);               // reads the pot and sets the timer to the potvalue
		ledtimer = map(timer, 0, 1023, ledmin, ledmax);   // maps the timer potvalue to the max and min vaues set above with x and y
	for (count = 0; count <= 4; count++) {     // cycles threw all the leds in the array
		digitalWrite(ledpinArray[count], HIGH);      // turns the first led on
				digitalWrite(redledpinArray[count], HIGH);      // turns the first led on
		delay(ledtimer);                             // sets the delay (for how fas it will go)
		digitalWrite(ledpinArray[count + 1], HIGH);  // turns the next led on
				digitalWrite(redledpinArray[count + 1], HIGH);      // turns the first led on
		delay(ledtimer);                             // sets the delay (for how fas it will go)
		digitalWrite(ledpinArray[count], LOW);       // turns it back off
				digitalWrite(redledpinArray[count], LOW);      // turns the first led on
		delay(ledtimer*1.5);                             // sets the delay (for how fas it will go)
	  }
	    for (count = 4; count > 0; count--) {    // same thing but in reverse so it will end up going back and forth
		digitalWrite(ledpinArray[count], HIGH);
			digitalWrite(redledpinArray[count], HIGH);      // turns the first led on
		delay(ledtimer);
		digitalWrite(ledpinArray[count - 1], HIGH);
			digitalWrite(redledpinArray[count - 1], HIGH);
		delay(ledtimer);
		digitalWrite(ledpinArray[count], LOW);
			digitalWrite(redledpinArray[count], LOW);
			delay(ledtimer*1.5);
	  }
	}
	void lightson() {
		digitalWrite(ledpinArray[count], HIGH);
		digitalWrite(redledpinArray[count], HIGH);
	}
	void lightsoff() {
	  		digitalWrite(ledpinArray[count], LOW);
	  		digitalWrite(redledpinArray[count], LOW);
	}
		void redon() {
		digitalWrite(redledpinArray[count], HIGH);
	}
		void redoff() {
	  	digitalWrite(redledpinArray[count], LOW);
	}
		void gedon() {
			digitalWrite(ledpinArray[count], HIGH);
	}
		void gedoff() {
	  		digitalWrite(ledpinArray[count], LOW);
	}
	void alternate(){
int ledtimer;
int timer;
timer = analogRead(potPin);               // reads the pot and sets the timer to the potvalue (this is the same as above but is reading it again so you don't have to waut as long to see the spd
		ledtimer = map(timer, 0, 1023, ledmin, ledmax);   // maps the timer potvalue to the max and min vaues set above with x and y
			digitalWrite(ledpinArray[count], HIGH);
			delay(ledtimer);
digitalWrite(redledpinArray[count], HIGH);
digitalWrite(ledpinArray[count], LOW);		
		delay(ledtimer*2);
		digitalWrite(ledpinArray[count], HIGH);
		digitalWrite(redledpinArray[count], LOW);
		delay(ledtimer*1.5);
		digitalWrite(ledpinArray[count], LOW);
		digitalWrite(redledpinArray[count], HIGH);
		}
	void blinklights() {
int timer;
int ledtimer;
timer = analogRead(potPin);               // reads the pot and sets the timer to the potvalue (this is the same as above but is reading it again so you don't have to waut as long to see the spd
		ledtimer = map(timer, 0, 1023, ledmin, ledmax);   // maps the timer potvalue to the max and min vaues set above with x and y
	  for (count = 0; count < 4; count++) {
		digitalWrite(ledpinArray[count], HIGH);
				digitalWrite(redledpinArray[count], HIGH);      // turns the first led on
		delay(ledtimer*2);
		digitalWrite(ledpinArray[count + 1], LOW);
	digitalWrite(redledpinArray[count + 1], LOW);
	delay(ledtimer*1.5);
		digitalWrite(ledpinArray[count], HIGH);
				digitalWrite(redledpinArray[count], HIGH);      // turns the first led on
		delay(ledtimer*1);
		digitalWrite(ledpinArray[count], LOW);
			digitalWrite(redledpinArray[count], LOW);      // turns the first led on
	  }
	}




// ** IGNORE FROM HERE, ONLY TEMP GAUGE INSTRUCTIONS...

void OneWireReset (int Pin) // reset.  Should improve to act as a presence pulse
{  digitalWrite (Pin, LOW);
  pinMode (Pin, OUTPUT);        // bring low for 500 us
  delayMicroseconds (500);
  pinMode (Pin, INPUT);
  delayMicroseconds (500);
}
void OneWireOutByte (int Pin, byte d) // output byte d (least sig bit first).
{
  byte n;
  for (n=8; n!=0; n--){
    if ((d & 0x01) == 1) {
      digitalWrite (Pin, LOW);
      pinMode (Pin, OUTPUT);
      delayMicroseconds (5);
      pinMode (Pin, INPUT);
      delayMicroseconds (60);
    }    else{
      digitalWrite (Pin, LOW);
      pinMode (Pin, OUTPUT);
      delayMicroseconds (60);
      pinMode (Pin, INPUT);
    }

    d = d>>1; // now the next bit is in the least sig bit position.
  }
}
byte OneWireInByte (int Pin) // read byte, least sig byte first
{
  byte d, n, b;
  for (n=0; n<8; n++)  {
    digitalWrite (Pin, LOW);
    pinMode (Pin, OUTPUT);
    delayMicroseconds (5);
    pinMode (Pin, INPUT);
    delayMicroseconds (5);
    b = digitalRead (Pin);
    delayMicroseconds (50);
    d = (d >> 1) | (b<<7);
	}
  return (d);
}
void getCurrentTemp (char *temp){
  int HighByte, LowByte, TReading, Tc_100, sign, whole, fract;
  OneWireReset (REF_PIN);
  OneWireOutByte (REF_PIN, 0xcc);
  OneWireOutByte (REF_PIN, 0x44); // perform temperature conversion, strong pullup for one sec
  OneWireReset (REF_PIN);
  OneWireOutByte (REF_PIN, 0xcc);
  OneWireOutByte (REF_PIN, 0xbe);
  LowByte = OneWireInByte (REF_PIN);
  HighByte = OneWireInByte (REF_PIN);
  TReading = (HighByte << 8) + LowByte;
  sign = TReading & 0x8000;  // test most sig bit
  if (sign) // negative  {
    TReading = (TReading ^ 0xffff) + 1;
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
  whole = Tc_100 / 100;  // separate off the whole and fractional portions
  fract = Tc_100 % 100;
  if (sign) {
    temp[0] = '-';
  } else {
    temp[0] = '+';  }
  if (whole/100 == 0) {
    temp[1] = ' ';  } else {
    temp[1] = whole/100+'0';
  }
  temp[2] = (whole-(whole/100)*100)/10 +'0' ;
  temp[3] = whole-(whole/10)*10 +'0';
  temp[4] = '.';
  temp[5] = fract/10 +'0';
  temp[6] = fract-(fract/10)*10 +'0';
  temp[7] = '\0';
  }

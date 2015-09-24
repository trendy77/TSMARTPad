#include <BH1750.h>
#include <Wire.h>
#include <SimpleTimer.h>
#include <Average.h>
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <OneWire.h>
// These are the interrupt and control pins for СС3000
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
BH1750 lightMeter;
#include <SPI.h>
#include <Adafruit_CC3000.h>
#include <BlynkSimpleCC3000.h>
int logged;int lg_light; int lg_temp;
const int lightMin = 0;const int lightMax = 1023;
int lightPercent;float celsius; float temp_c; 
String temperature; uint16_t lux;
Average<float> aveLL(10);
static char tempbuffer[10];
Average<float> aveLT(10);
OneWire ds(A1);  
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "e614f52e3e0d4a879b968a8491abe9ed";
SimpleTimer timer;
// Attach virtual serial terminal to virtual pin 1
WidgetTerminal terminal(1);

void setup()
{
  Serial.begin(115200);
  Serial.println("here goes nothing");
  timer.setInterval(1000L, sendUptime);
   lightMeter.begin();
  lux = lightMeter.readLightLevel();
  lg_light = lux;
  Blynk.begin(auth, "LANoftheTriBand", "tttttttt", WLAN_SEC_WPA2);
  while (Blynk.connect() == false) {
    // Wait until connected
  }

  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.flush();
}

void loop()
{
 readSensors();
 
 Blynk.run();
  timer.run(); // Initiates SimpleTimer
}


void sendUptime()
{
    
  Blynk.virtualWrite(V2, temp_c);
  Blynk.virtualWrite(V3, lg_light);
  delay(100);
}



void readSensors() {
lux = lightMeter.readLightLevel();
lg_light = lux;
//	lightPercent = map(lux, lightMin, lightMax, 0, 100);
	byte i;
	byte present = 0;
	byte type_s;
	byte data[12];
	byte addr[8];
	if (!ds.search(addr)) {
		//    Serial.println("No more addresses.");
//		Serial.println("*");
		ds.reset_search();
		delay(250);
		return;
	}
	//Serial.println();
	// the first ROM byte indicates which chip
	switch (addr[0]) {
	case 0x10:
//		Serial.println("  Chip = DS18S20");  // or old DS1820
		type_s = 1;
		break;
	case 0x28:
//		Serial.println("  Chip = DS18B20");
		type_s = 0;
		break;
	case 0x22:
//		Serial.println("  Chip = DS1822");
		type_s = 0;
		break;
	// Serial.print("ROM =");
	//  for( i = 0; i < 8; i++) {
	//  Serial.write(' ');
	//   Serial.print(addr[i], HEX);
	// }
	if (OneWire::crc8(addr, 7) != addr[7]) {
		Serial.println("CRC is not valid!");
		return;
	}break;
	default:
//		Serial.println("Device is not a DS18x20 family device.");
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
//	Serial.print("  Data = ");
//	Serial.print(present, HEX);
//	Serial.print(" ");
	for (i = 0; i < 9; i++) {           // we need 9 bytes
		data[i] = ds.read();
//		Serial.print(data[i], HEX);
//		Serial.print(" ");
	}
//	Serial.print(" CRC=");
//	Serial.print(OneWire::crc8(data, 8), HEX);
//	Serial.println("**");

	// Convert the data to actual temperature
	// because the result is a 16 bit signed integer, it should
	// be stored to an "int16_t" type, which is always 16 bits
	// even when compiled on a 32 bit processor.
	int16_t raw = (data[1] << 8) | data[0];
	if (type_s) {
		raw = raw << 3; // 9 bit resolution default
		if (data[7] == 0x10) {
			// "count remain" gives full 12 bit resolution
			raw = (raw & 0xFFF0) + 12 - data[6];
		}
	}
	else {	
	dtostrf(temp_c, 6, 2, tempbuffer);
	//  dtostrf(humid,6, 2, humidbuffer);
	temperature = tempbuffer;
	temperature.trim();
	//  String humidity = humidbuffer;
	//   humidity.trim();
}
}
void printSensors(){

	Serial.print("Temperature: ");
	Serial.print(celsius);
	Serial.println("C ");
	Serial.print("Light Lux=");  Serial.print(lg_light);
	Serial.print(" or ");  Serial.print(lightPercent);  Serial.println( "% ");
	Serial.println("  ");
}

void readAndPrint(){
	readSensors();
	printSensors();
}
  /*
void beep(int num){
	for (int i = 0; i < num; i++){
		digitalWrite(piezoPin, HIGH);
		delay(500);
		digitalWrite(piezoPin, LOW);
		delay(500);
	}
}

void kitchWinFwd(){
	Serial.print(" kitchWindow Opening "); Serial.print(wintimerS); Serial.println(" secs");  	  leds.setLEDs(LED_YELLOW);
	 leds.update();
	digitalWrite(kitchWinF, LOW);
	digitalWrite(kitchWinR, HIGH);
	delay(wintimer);
	digitalWrite(kitchWinR, LOW);
}
void kitchWinBwd(){
	leds.setLEDs(LED_YELLOW);
	leds.update(); Serial.println(" kitchWindow Closing "); Serial.print(wintimerS); Serial.println(" secs");
	digitalWrite(kitchWinR, LOW);	digitalWrite(kitchWinF, HIGH);	delay(wintimer);	digitalWrite(kitchWinF, LOW);
}

void loungeWinFwd(){
digitalWrite(loungeWinR, LOW);
digitalWrite(loungeWinF, HIGH);
delay(wintimer);
digitalWrite(loungeWinF, LOW);
}
void loungeWinBwd(){
digitalWrite(loungeWinF, LOW);
digitalWrite(loungeWinR, HIGH);
delay(wintimer);
//winState = 1;
digitalWrite(loungeWinR, LOW);
}

void send2server(){
	wdt_reset(); 
	Serial.println("Time to send server");
	readAndPrint();
	String request = "GET " + repository + "sensor.php?lg_temp=" + temperature + "&lg_light=" + lg_light + " HTTP/1.0";
	send_request(request);
}
*/
BLYNK_WRITE(V1)
{
  BLYNK_LOG("Hey! FUCK ME I new message!");

  // Send it back
  terminal.print("I got:");
  terminal.write(param.getBuffer(), param.getLength());
  terminal.println();

  // Ensure everything is sent
  terminal.flush();
}


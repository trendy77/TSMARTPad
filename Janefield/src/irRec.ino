/* comboDump.ino Example sketch for IRLib2
 *  Illustrate how to create a custom decoder using only the protocols
 *  you wish to use.
 #include <IRLibDecodeBase.h> // First include the decode base
#include <IRLib_P01_NEC.h>   // Now include only the protocols you wish
#include <IRLib_P02_Sony.h>  // to actually use. The lowest numbered
#include <IRLib_P07_NECx.h>  // must be first but others can be any order.
#include <IRLib_P09_GICable.h>
#include <IRLib_P11_RCMM.h>
#include <IRLibCombo.h>     // After all protocols, include this
*/



// Include a receiver either this or IRLibRecvPCI or IRLibRecvLoop
#include <IRLibRecv.h>

void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
}
void loop() {
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    myDecoder.dumpResults(true);  //Now print results. Use false for less detail
    myReceiver.enableIRIn();      //Restart receiver
  }
}

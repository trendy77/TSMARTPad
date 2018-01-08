
void loop() {
 
  // Make sure to reset watchdog every loop iteration!
  Watchdog.reset();
 
 
 
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }
 
  // grab the current state of the windows
  bobdistance = distance;
  rmwindist = distance2;
 
  // return if the value hasn't changed
  if(bobdistance == distance)
    return;
 
  int32_t value = (current == LOW ? 1 : 0);
 
  // Now we can publish stuff!
  Serial.print(F("\nSending button value: "));
  Serial.print(value);
  Serial.print("... ");
 
  if (! button.publish(value))
    Serial.println(F("Failed."));
  else
    Serial.println(F("Success!"));
 
  // save the button state
  last = current;
 
}
/*
 * Function:  set_pins
 * --------------------
 * sets pins for leds * dht sensor
 *
 *  returns: nothing, void func
 */

void set_pins(){
  pinMode(Ld1_redPin, OUTPUT);
  pinMode(Ld1_greenPin, OUTPUT);
  pinMode(Ld1_bluePin, OUTPUT);
  pinMode(Ld2_redPin, OUTPUT);
  pinMode(DHTPIN, INPUT);
}

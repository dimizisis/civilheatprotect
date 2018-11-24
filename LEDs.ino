/*
 * Function:  led_status 
 * --------------------
 * sets LED color, according to DI
 *
 *  DI: Discomfort Index value
 *
 *  returns: nothing, void func
 *  
 */

void led_status(int DI){

  if (DI==1){
    setColor1(0, 255, 0);   // Green Color
    setColor2(LOW);           // No Color
  }else if (DI==2){
    setColor1(0, 0, 255); // Blue Color
    setColor2(LOW);         // No Color
  }else if (DI==3){
    setColor1(255, 0, 0); // Red Color
    setColor2(LOW);         // No Color
  }else if (DI==4){
    setColor1(0, 255, 0);   // Green Color
    setColor2(HIGH);         // Red Color
  }else if (DI==5){
    setColor1(0, 0, 255);   // Blue Color
    setColor2(HIGH);         // Red Color
  }else if (DI==6){
    setColor1(255, 0, 0);   // Red Color
    setColor2(HIGH);         // Red Color
  }
  delay(delay_time);
}

/*
 * Function:  switchoff
 * --------------------
 * switches off LEDs
 *
 *  returns: nothing, void func
 *  
 */

void switchoff(){
  setColor1(0, 0, 0);   // No Color
  setColor2(LOW);       // No Color
  delay(2000);
}

/*
 * Function:  setColor1 
 * --------------------
 * sets RGB LED color
 *
 *  redValue: red value (0 - 255)
 *  greenValue: green value (0 - 255)
 *  blueValue: blue value (0 - 255)
 *
 *  returns: nothing, void func
 *  
 */

void setColor1(int redValue, int greenValue, int blueValue) {
  analogWrite(Ld1_redPin, redValue);
  analogWrite(Ld1_greenPin, greenValue);
  analogWrite(Ld1_bluePin, blueValue);
}

/*
 * Function:  setColor2 
 * --------------------
 * sets red LED color
 *
 *  redValue: red value (0 - 255)
 *
 *  returns: nothing, void func
 *  
 */

void setColor2(int redValue) {
  digitalWrite(Ld2_redPin, redValue);
}

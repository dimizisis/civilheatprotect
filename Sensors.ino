/*
 * Function:  get_data_from_sensor
 * --------------------
 * gets data (temperature & humidity) from DHT sensor
 * 
 * avgC: pointer to temperature variable, the one that will be used to calculate the average temp
 * avgH: pointer to humidity variable, the one that will be used to calculate the average hum
 * obs: pointer to number of observations variable, the one that will be used to divide avgC, avgH with
 * min_temp: pointer to minimum temperature variable (from all observations)
 * max_temp: pointer to maximum temperature variable (from all observations)
 * min_hum: pointer to minimum humidity variable (from all observations)
 * max_hum: pointer to maximum humidity variable (from all observations)
 *
 *  returns true, if sensors available & working
 *          false, if sensors failed to sample
 *  
 */

bool get_data_from_sensor(float * avgC, float * avgH, int * obs, float * min_temp, float * max_temp, float * min_hum, float * max_hum){

  /* Initializing min & max variables */
  
  (*max_temp) = sensor.min_temp;
  (*min_temp) = sensor.max_temp;

  (*max_hum) = sensor.min_hum;
  (*min_hum) = sensor.max_hum;

  float temp, hum;

  for(int i=0;i<reads_per_frame; ++i){
            
    temp = dht.readTemperature();
            
    hum = dht.readHumidity();

    Serial.println("Temp: "+String(temp)+" Hum: "+String(hum));
            
    if ((!isnan(temp) && !isnan(hum))) {
      if ((temp >= sensor.min_temp && temp <= sensor.max_temp) && (hum >= sensor.min_hum && hum <= sensor.max_hum)){
        (*avgC) += temp;
        (*avgH) += hum;
        ++(*obs);
        update_min_max(temp, hum, min_temp, max_temp, min_hum, max_hum);
     }
     else{
      if (get_valid_num(&temp, &hum) == 0){ 
        (*avgC) += temp;
        (*avgH) += hum;
        ++(*obs);
        update_min_max(temp, hum, min_temp, max_temp, min_hum, max_hum);
      }
      else 
        return false;
     }
   }
   else{
    if (get_valid_num(&temp, &hum) == 0){
      (*avgC) += temp;
      (*avgH) += hum; 
      ++(*obs);
      update_min_max(temp, hum, min_temp, max_temp, min_hum, max_hum);
    }
    else 
      return false;
   }  
   delay(time_frame/reads_per_frame); // Wait n seconds before accessing sensor again.
  }
  
  return true;
}

/*
 * Function:  update_min_max
 * --------------------
 * checks each observation and updates (if needed) minimum & maximum value (temp & hum)
 *
 *  temp: temperature that being checked
 *  hum: humidity that being checked
 *  min_temp: pointer to minimum temperature variable (from all observations)
 *  max_temp: pointer to maximum temperature variable (from all observations)
 *  min_hum: pointer to minimum humidity variable (from all observations)
 *  max_hum: pointer to maximum humidity variable (from all observations)
 *
 *  returns: nothing, void func
 */

void update_min_max(float temp, float hum, float * min_temp, float * max_temp, float * min_hum, float * max_hum){
  
  if ((*min_temp) > temp)
    (*min_temp)=temp;

  if ((*max_temp) < temp)
    (*max_temp)=temp;

  if ((*min_hum) > hum)
    (*min_hum)=hum;

  if ((*max_hum) < hum)
    (*max_hum)=hum;
}

/*
 * Function:  get_valid_num
 * --------------------
 * tries to get valid data from sensor (temp & hum) for 6 seconds
 *
 *  temp: pointer to variable of temperature
 *  hum: pointer to variable of humidity
 *
 *  returns: 0, if successfully got valid data (temp & hum)
 *          -1, if couldn't get valid data (temp & hum) in 6 seconds
 */

int get_valid_num(float * temp, float * hum){

  uint32_t curr_time = millis(); // curr_time has how many milliseconds the device is in operation
  while (true){
    if (millis() - curr_time > STANDARD_DELAY_TIME*2){ // if it reaches timeout, return
      PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR, 0, 0, true, true);
      return -1;
    }
    (*temp) = dht.readTemperature();
    (*hum) = dht.readHumidity();
    Serial.println("INSIDE WHILE: Temp: "+String((*temp))+" Hum: "+String((*hum)));
    delay(1000);
    if ((!isnan((*temp)) && !isnan((*hum)))) {
      if (((*temp) >= sensor.min_temp && (*temp) <= sensor.max_temp) && ((*hum) >= sensor.min_hum && (*hum) <= sensor.max_hum))
         return 0;
    }         
    delay(1000);
 }
}

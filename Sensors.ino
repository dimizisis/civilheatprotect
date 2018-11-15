bool get_data_from_sensors(int flag, float * avg1C, float * avg2C, float * avg1H, float * avg2H, int * DI){

  int s1=0,s2=0;
  int DI_temp=0;
  float min_temp=DHT22_MAX_TEMP;
  float max_temp=DHT22_MIN_TEMP;
  float min_hum=DHT22_MAX_HUM;
  float max_hum=DHT22_MIN_HUM;
  float min_temp2=DHT11_MAX_TEMP;
  float max_temp2=DHT11_MIN_TEMP;
  float min_hum2=DHT11_MAX_HUM;
  float max_hum2=DHT11_MIN_HUM;

  // If SD card & WiFi OK
  if (flag == 0){

    if (main_sensor == 22){
      min_temp=DHT22_MAX_TEMP;
      max_temp=DHT22_MIN_TEMP;
      min_hum=DHT22_MAX_HUM;
      max_hum=DHT22_MIN_HUM;
      for(int i=0;i<readsperframe; ++i){
        
            if (main_sensor != 22) break; // If main sensor changed, break.
            
            temp = dht1.readTemperature();
            
            hum = dht1.readHumidity();
            
            if ((!isnan(temp) && !isnan(hum))) {
              if ((temp >= DHT22_MIN_TEMP && temp <= DHT22_MAX_TEMP) && (hum >= DHT22_MIN_HUM && hum <= DHT22_MAX_HUM)){
                (*avg1C) += temp;
                (*avg1H) += hum;
                ++s1;
                if (min_temp > temp)
                  min_temp=temp;

                if (max_temp < temp)
                  max_temp=temp;

                if (min_hum > hum)
                  min_hum=temp;

                if (max_hum < hum)
                  max_hum=hum;
              }
              else{
                uint32_t curr_time = millis();
                while (true){
                  if (millis() - curr_time > STANDARD_DELAY_TIME*2){
                    main_sensor = -1;
                    PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR, true);
                    break;
                  }
                  temp = dht1.readTemperature();
                  hum = dht1.readHumidity();
                  if ((!isnan(temp) && !isnan(hum))) {
                    if ((temp >= DHT22_MIN_TEMP && temp <= DHT22_MAX_TEMP) && (hum >= DHT22_MIN_HUM && hum <= DHT22_MAX_HUM))
                      break;
                }         
                delay(STANDARD_DELAY_TIME);
              }
              if (main_sensor != -1){
                if (min_temp > temp)
                  min_temp=temp;

                if (max_temp < temp)
                  max_temp=temp;

                if (min_hum > hum)
                  min_hum=temp;

                if (max_hum < hum)
                  max_hum=hum;
              }
            }
            }
            else{
                uint32_t curr_time = millis();
                while (true){
                  if (millis() - curr_time > STANDARD_DELAY_TIME*2){
                    main_sensor = -1;
                    PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR, true);
                    break;
                  }
                  temp = dht1.readTemperature();
                  hum = dht1.readHumidity();
                  if ((!isnan(temp) && !isnan(hum))) {
                    if ((temp >= DHT22_MIN_TEMP && temp <= DHT22_MAX_TEMP) && (hum >= DHT22_MIN_HUM && hum <= DHT22_MAX_HUM))
                      break;
                }         
                delay(STANDARD_DELAY_TIME);
              }
              if (main_sensor != -1){
                if (min_temp > temp)
                  min_temp=temp;

                if (max_temp < temp)
                  max_temp=temp;

                if (min_hum > hum)
                  min_hum=temp;

                if (max_hum < hum)
                  max_hum=hum;
              }
            }
            delay(timeframe/readsperframe); // Wait n seconds before accessing sensor again.
            }
      }
    if (main_sensor == 11){
      min_temp2=DHT11_MAX_TEMP;
      max_temp2=DHT11_MIN_TEMP;
      min_hum2=DHT11_MAX_HUM;
      max_hum2=DHT11_MIN_HUM;
      for(int i=0;i<readsperframe; ++i){
          if (main_sensor != 11) break;
          temp2= dht1.readTemperature();
          
          hum2 = dht1.readHumidity();
          
          if ((!isnan(temp2) && !isnan(hum2))) {
              if ((temp2 >= DHT11_MIN_TEMP && temp2 <= DHT11_MAX_TEMP) && (hum2 >= DHT11_MIN_HUM && hum2 <= DHT11_MAX_HUM)){
                (*avg2C) += temp2;
                (*avg2H) += hum2;
                ++s2;

                if (min_temp2 > temp2)
                  min_temp2=temp2;

                if (max_temp2 < temp2)
                  max_temp2=temp2;

                if (min_hum2 > hum2)
                  min_hum2=temp2;

                if (max_hum2 < hum2)
                  max_hum2=hum2;
              }
              else{
          
                /* Try & get sensor values for max time the standard delay we set. If still nan or out of bounds, then sensors unavailable... */
                uint32_t curr_time = millis();
                while (true){
                  if (millis() - curr_time > STANDARD_DELAY_TIME*2){
                    main_sensor = -1;
                    PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR, true);
                    break;
                  }
                  temp = dht1.readTemperature();
                  hum = dht1.readHumidity();
                  if ((!isnan(temp) && !isnan(hum))) {
                    if ((temp >= DHT11_MIN_TEMP && temp <= DHT11_MAX_TEMP) && (hum >= DHT11_MIN_HUM && hum <= DHT11_MAX_HUM))
                      break;
                }
                
                delay(STANDARD_DELAY_TIME);
              }
              if (main_sensor != -1){
                if (min_temp2 > temp2)
                  min_temp2=temp2;

                if (max_temp2 < temp2)
                  max_temp2=temp2;

                if (min_hum2 > hum2)
                  min_hum2=temp2;

                if (max_hum2 < hum2)
                  max_hum2=hum2;
              }
            }
          }
         else{
          
                /* Try & get sensor values for max time the standard delay we set. If still nan or out of bounds, then sensors unavailable... */
                uint32_t curr_time = millis();
                while (true){
                  if (millis() - curr_time > STANDARD_DELAY_TIME*2){
                    main_sensor = -1;
                    PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR, true);
                    break;
                  }
                  temp = dht1.readTemperature();
                  hum = dht1.readHumidity();
                  if ((!isnan(temp) && !isnan(hum))) {
                    if ((temp >= DHT11_MIN_TEMP && temp <= DHT11_MAX_TEMP) && (hum >= DHT11_MIN_HUM && hum <= DHT11_MAX_HUM))
                      break;
                }
                
                delay(STANDARD_DELAY_TIME);
              }
              if (main_sensor != -1){
                if (min_temp2 > temp2)
                  min_temp2=temp2;

                if (max_temp2 < temp2)
                  max_temp2=temp2;

                if (min_hum2 > hum2)
                  min_hum2=temp2;

                if (max_hum2 < hum2)
                  max_hum2=hum2;
              }
            }
          delay(timeframe/readsperframe); // Wait n seconds before accessing sensor again.
       }
    }
    if (main_sensor==22){
      /* Remove min & max temperature & humidity values from averages (for better sampling) */
      (*avg1C) -= max_temp;
      (*avg1C) -= min_temp;
      (*avg1H) -= max_hum;
      (*avg1H) -= min_hum;
      s1 -= 2;
      (*avg1C) /= s1;
      (*avg1H) /= s1;
      DI_temp= round((*avg1C) - 0.55 * (1 - 0.01 * (*avg1H)) * ((*avg1C) - 14.5));

      PRINT_TO_USER(SEPARATOR, LCD_EMPTY, true);
      PRINT_TO_USER(IN_CONDITIONS, LCD_EMPTY, true);
      PRINT_TO_USER(TEMPERATURE, LCD_TEMPERATURE, true);
      delay(STANDARD_DELAY_TIME);
      print_to_user(String((*avg1C)), String((*avg1C)), true);
      delay(STANDARD_DELAY_TIME);
//      lcd.clear();
      PRINT_TO_USER(HUMIDITY, LCD_HUMIDITY, true);
      delay(STANDARD_DELAY_TIME);
      print_to_user(String((*avg1H)), String((*avg1H)), true);
    }
    else if (main_sensor==11){
      /* Remove min & max temperature & humidity values from averages (for better sampling) */
      (*avg2C) -= max_temp2;
      (*avg2C) -= min_temp2;
      (*avg2H) -= max_hum2;
      (*avg2H) -= min_hum2;
      s2 -= 2;
      (*avg2C) /= s2;
      (*avg2H) /= s2;
      DI_temp= round((*avg2C) - 0.55 * (1 - 0.01 * (*avg2H)) * ((*avg2C) - 14.5));

      PRINT_TO_USER(SEPARATOR, LCD_EMPTY, true);
      PRINT_TO_USER(IN_CONDITIONS, LCD_EMPTY, true);
      PRINT_TO_USER(TEMPERATURE, LCD_TEMPERATURE, true);
      delay(STANDARD_DELAY_TIME);
      print_to_user(String((*avg2C)), String((*avg2C)), true);
      delay(STANDARD_DELAY_TIME);
//      lcd.clear();
      PRINT_TO_USER(HUMIDITY, LCD_HUMIDITY, true);
      delay(STANDARD_DELAY_TIME);
      print_to_user(String((*avg2H)), String((*avg2H)), true);
    }
    else{
      PRINT_TO_USER(SENSORS_UNAVAILABLE, LCD_SENSORS_UNAVAILABLE, true);
      delay(STANDARD_DELAY_TIME);
      PRINT_TO_USER(RESET, LCD_RESET, true);
      delay(STANDARD_DELAY_TIME);
      return false;
    }

    *DI = calc_DI(DI_temp);
    led_status(*DI);
    return true;

  }

  // If SD card OK, WiFi failed to connect
  else if (flag == 1){


  }

  // If SD card failed to initialize & WiFi failed to connect
  else{

    if (main_sensor == 22){
      min_temp=DHT22_MAX_TEMP;
      max_temp=DHT22_MIN_TEMP;
      min_hum=DHT22_MAX_HUM;
      max_hum=DHT22_MIN_HUM;
      for(int i=0;i<readsperframe; ++i){
        
            if (main_sensor != 22) break; // If main sensor changed, break.
            
            temp = dht1.readTemperature();
            
            hum = dht1.readHumidity();
            
            if ((!isnan(temp) && !isnan(hum))) {
              if ((temp >= DHT22_MIN_TEMP && temp <= DHT22_MAX_TEMP) && (hum >= DHT22_MIN_HUM && hum <= DHT22_MAX_HUM)){
                (*avg1C) += temp;
                (*avg1H) += hum;
                ++s1;
                if (min_temp > temp)
                  min_temp=temp;

                if (max_temp < temp)
                  max_temp=temp;

                if (min_hum > hum)
                  min_hum=temp;

                if (max_hum < hum)
                  max_hum=hum;
              }
              else{
                uint32_t curr_time = millis();
                while (true){
                  if (millis() - curr_time > STANDARD_DELAY_TIME*2){
                    main_sensor = -1;
                    PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR, true);
                    break;
                  }
                  temp = dht1.readTemperature();
                  hum = dht1.readHumidity();
                  if ((!isnan(temp) && !isnan(hum))) {
                    if ((temp >= DHT22_MIN_TEMP && temp <= DHT22_MAX_TEMP) && (hum >= DHT22_MIN_HUM && hum <= DHT22_MAX_HUM))
                      break;
                }         
                delay(STANDARD_DELAY_TIME);
              }
              if (main_sensor != -1){
                if (min_temp > temp)
                  min_temp=temp;

                if (max_temp < temp)
                  max_temp=temp;

                if (min_hum > hum)
                  min_hum=temp;

                if (max_hum < hum)
                  max_hum=hum;
              }
            }
            }
            else{
                uint32_t curr_time = millis();
                while (true){
                  if (millis() - curr_time > STANDARD_DELAY_TIME*2){
                    main_sensor = -1;
                    PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR, true);
                    break;
                  }
                  temp = dht1.readTemperature();
                  hum = dht1.readHumidity();
                  if ((!isnan(temp) && !isnan(hum))) {
                    if ((temp >= DHT22_MIN_TEMP && temp <= DHT22_MAX_TEMP) && (hum >= DHT22_MIN_HUM && hum <= DHT22_MAX_HUM))
                      break;
                }         
                delay(STANDARD_DELAY_TIME);
              }
              if (main_sensor != -1){
                if (min_temp > temp)
                  min_temp=temp;

                if (max_temp < temp)
                  max_temp=temp;

                if (min_hum > hum)
                  min_hum=temp;

                if (max_hum < hum)
                  max_hum=hum;
              }
            }
            delay(timeframe/readsperframe); // Wait n seconds before accessing sensor again.
            }
      }
    if (main_sensor == 11){
      min_temp2=DHT11_MAX_TEMP;
      max_temp2=DHT11_MIN_TEMP;
      min_hum2=DHT11_MAX_HUM;
      max_hum2=DHT11_MIN_HUM;
      for(int i=0;i<readsperframe; ++i){
          if (main_sensor != 11) break;
          temp2= dht1.readTemperature();
          
          hum2 = dht1.readHumidity();
          
          if ((!isnan(temp2) && !isnan(hum2))) {
              if ((temp2 >= DHT11_MIN_TEMP && temp2 <= DHT11_MAX_TEMP) && (hum2 >= DHT11_MIN_HUM && hum2 <= DHT11_MAX_HUM)){
                (*avg2C) += temp2;
                (*avg2H) += hum2;
                ++s2;

                if (min_temp2 > temp2)
                  min_temp2=temp2;

                if (max_temp2 < temp2)
                  max_temp2=temp2;

                if (min_hum2 > hum2)
                  min_hum2=temp2;

                if (max_hum2 < hum2)
                  max_hum2=hum2;
              }
              else{
          
                /* Try & get sensor values for max time the standard delay we set. If still nan or out of bounds, then sensors unavailable... */
                uint32_t curr_time = millis();
                while (true){
                  if (millis() - curr_time > STANDARD_DELAY_TIME*2){
                    main_sensor = -1;
                    PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR, true);
                    break;
                  }
                  temp = dht1.readTemperature();
                  hum = dht1.readHumidity();
                  if ((!isnan(temp) && !isnan(hum))) {
                    if ((temp >= DHT11_MIN_TEMP && temp <= DHT11_MAX_TEMP) && (hum >= DHT11_MIN_HUM && hum <= DHT11_MAX_HUM))
                      break;
                }
                delay(STANDARD_DELAY_TIME);
              }
              if (main_sensor != -1){
                if (min_temp2 > temp2)
                  min_temp2=temp2;

                if (max_temp2 < temp2)
                  max_temp2=temp2;

                if (min_hum2 > hum2)
                  min_hum2=temp2;

                if (max_hum2 < hum2)
                  max_hum2=hum2;
              }
            }
          }
         else{
          
                /* Try & get sensor values for max time the standard delay we set. If still nan or out of bounds, then sensors unavailable... */
                uint32_t curr_time = millis();
                while (true){
                  if (millis() - curr_time > STANDARD_DELAY_TIME*2){
                    main_sensor = -1;
                    PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR, true);
                    break;
                  }
                  temp = dht1.readTemperature();
                  hum = dht1.readHumidity();
                  if ((!isnan(temp) && !isnan(hum))) {
                    if ((temp >= DHT11_MIN_TEMP && temp <= DHT11_MAX_TEMP) && (hum >= DHT11_MIN_HUM && hum <= DHT11_MAX_HUM))
                      break;
                }
                delay(STANDARD_DELAY_TIME);
              }
              if (main_sensor != -1){
                if (min_temp2 > temp2)
                  min_temp2=temp2;

                if (max_temp2 < temp2)
                  max_temp2=temp2;

                if (min_hum2 > hum2)
                  min_hum2=temp2;

                if (max_hum2 < hum2)
                  max_hum2=hum2;
              }
            }
          delay(timeframe/readsperframe); // Wait n seconds before accessing sensor again.
       }
    }
    if (main_sensor==22){
      /* Remove min & max temperature & humidity values from averages (for better sampling) */
      (*avg1C) -= max_temp;
      (*avg1C) -= min_temp;
      (*avg1H) -= max_hum;
      (*avg1H) -= min_hum;
      s1 -= 2;
      (*avg1C) /= s1;
      (*avg1H) /= s1;
      DI_temp= round((*avg1C) - 0.55 * (1 - 0.01 * (*avg1H)) * ((*avg1C) - 14.5));

      PRINT_TO_USER(SEPARATOR, LCD_EMPTY, true);
      PRINT_TO_USER(IN_CONDITIONS, LCD_EMPTY, true);
      PRINT_TO_USER(TEMPERATURE, LCD_TEMPERATURE, true);
      delay(STANDARD_DELAY_TIME);
      print_to_user(String((*avg1C)), String((*avg1C)), true);
      delay(STANDARD_DELAY_TIME);
//      lcd.clear();
      PRINT_TO_USER(HUMIDITY, LCD_HUMIDITY, true);
      delay(STANDARD_DELAY_TIME);
      print_to_user(String((*avg1H)), String((*avg1H)), true);
    }
    else if (main_sensor==11){
      /* Remove min & max temperature & humidity values from averages (for better sampling) */
      Serial.println("avg2C (before): "+String((*avg2C)));
      (*avg2C) -= max_temp2;
      (*avg2C) -= min_temp2;
      Serial.println("min_temp2: " + String(min_temp2) + " max_temp2: "+String(max_temp2));
      Serial.println("avg2C (after): "+String((*avg2C)));
      (*avg2H) -= max_hum2;
      (*avg2H) -= min_hum2;
      s2 -= 2;
      (*avg2C) /= s2;
      (*avg2H) /= s2;
      DI_temp= round((*avg2C) - 0.55 * (1 - 0.01 * (*avg2H)) * ((*avg2C) - 14.5));

      PRINT_TO_USER(SEPARATOR, LCD_EMPTY, true);
      PRINT_TO_USER(IN_CONDITIONS, LCD_EMPTY, true);
      PRINT_TO_USER(TEMPERATURE, LCD_TEMPERATURE, true);
      delay(STANDARD_DELAY_TIME);
      print_to_user(String((*avg2C)), String((*avg2C)), true);
      delay(STANDARD_DELAY_TIME);
//      lcd.clear();
      PRINT_TO_USER(HUMIDITY, LCD_HUMIDITY, true);
      delay(STANDARD_DELAY_TIME);
      print_to_user(String((*avg2H)), String((*avg2H)), true);
    }
    else{
      PRINT_TO_USER(SENSORS_UNAVAILABLE, LCD_SENSORS_UNAVAILABLE, true);
      delay(STANDARD_DELAY_TIME);
      PRINT_TO_USER(RESET, LCD_RESET, true);
      delay(STANDARD_DELAY_TIME);
      return false;
    }

    *DI = calc_DI(DI_temp);
    led_status(*DI);
    return true;

}

}

//int get_sensor_num(){
//
//  int sensor_num;
//
//  /*
//   *  Returns 1 if sensor is DHT22
//   *  Returns 2 if sensor is DHT11
//   */
//
//  if (String(buffer) == "DHT22")
//    sensor_num = 1;
//  else if (String(buffer) == "DHT11")
//    sensor_num = 2;
//
//  return sensor_num;
//}

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
            
            temp= dht1.readTemperature();
            Serial.println("TEMP: "+String(temp));

            if (min_temp > temp)
              min_temp=temp;

            if (max_temp < temp)
              max_temp=temp;
            
            hum = dht1.readHumidity();

            if (min_hum > hum)
              min_hum=temp;

            if (max_hum < hum)
              max_hum=hum;
            
            if ((!isnan(temp) && !isnan(hum))) {
              if ((temp >= DHT22_MIN_TEMP && temp <= DHT22_MAX_TEMP) && (hum >= DHT22_MIN_HUM && hum <= DHT22_MAX_HUM)){
                (*avg1C) += temp;
                (*avg1H) += hum;
                ++s1;
              }
              else{
                main_sensor = -1;
                PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR);
                delay(STANDARD_DELAY_TIME);
//                PRINT_TO_USER(USING_DHT11, LCD_USING_DHT11);
//                delay(STANDARD_DELAY_TIME);
              }
            }else{
              PRINT_TO_USER(UNAVAILABLE_DHT22, LCD_UNAVAILABLE_DHT22);
              main_sensor = -1; // Make DHT11 main sensor (DHT22 not available)
//              delay(STANDARD_DELAY_TIME);
//              PRINT_TO_USER(USING_DHT11, LCD_USING_DHT11);
              delay(STANDARD_DELAY_TIME);
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
          Serial.println("TEMP2: "+String(temp2));

          if (min_temp2 > temp2)
              min_temp2=temp;

            if (max_temp2 < temp2)
              max_temp2=temp2;
          
          hum2 = dht1.readHumidity();

          if (min_hum2 > hum2)
              min_hum2=temp2;

            if (max_hum2 < hum2)
              max_hum2=hum2;

          Serial.println("MAX_TEMP2: "+String(max_temp2));
          
          if ((!isnan(temp2) && !isnan(hum2))) {
              if ((temp2 >= DHT11_MIN_TEMP && temp2 <= DHT11_MAX_TEMP) && (hum2 >= DHT11_MIN_HUM && hum2 <= DHT11_MAX_HUM)){
                (*avg2C) += temp2;
                (*avg2H) += hum2;
                ++s2;
              }
              else{
                PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR);
                main_sensor = -1;
                delay(STANDARD_DELAY_TIME);
              }
          }else{
            PRINT_TO_USER(UNAVAILABLE_DHT11, LCD_UNAVAILABLE_DHT11);
            main_sensor = -1; // -1 if none of sensors are available
            delay(STANDARD_DELAY_TIME);
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

      PRINT_TO_USER(SEPARATOR, LCD_EMPTY);
      PRINT_TO_USER(IN_CONDITIONS, LCD_EMPTY);
      PRINT_TO_USER(TEMPERATURE, LCD_TEMPERATURE);
      print_to_user(String((*avg1C)), String((*avg1C)));
      delay(STANDARD_DELAY_TIME);
//      lcd.clear();
      PRINT_TO_USER(HUMIDITY, LCD_HUMIDITY);
      print_to_user(String((*avg1H)), String((*avg1H)));
    }
    else if (main_sensor==11){
      /* Remove min & max temperature & humidity values from averages (for better sampling) */
      Serial.println("avg2C (before): "+String((*avg2C)));
      (*avg2C) -= max_temp2;
      (*avg2C) -= min_temp2;
      Serial.println("avg2C (after): "+String((*avg2C)));
      (*avg2H) -= max_hum2;
      (*avg2H) -= min_hum2;
      s2 -= 2;
      (*avg2C) /= s2;
      (*avg2H) /= s2;
      DI_temp= round((*avg2C) - 0.55 * (1 - 0.01 * (*avg2H)) * ((*avg2C) - 14.5));

      PRINT_TO_USER(SEPARATOR, LCD_EMPTY);
      PRINT_TO_USER(IN_CONDITIONS, LCD_EMPTY);
      PRINT_TO_USER(TEMPERATURE, LCD_TEMPERATURE);
      print_to_user(String((*avg2C)), String((*avg2C)));
      delay(STANDARD_DELAY_TIME);
//      lcd.clear();
      PRINT_TO_USER(HUMIDITY, LCD_HUMIDITY);
      print_to_user(String((*avg2H)), String((*avg2H)));
    }
    else{
      PRINT_TO_USER(SENSORS_UNAVAILABLE, LCD_SENSORS_UNAVAILABLE);
      delay(STANDARD_DELAY_TIME);
      PRINT_TO_USER(RESET, LCD_RESET);
      delay(STANDARD_DELAY_TIME);
      return false;
    }

    *DI = calc_DI(DI_temp);
//      write_data_to_server( iot_server, channelID , df1, (*avg1C) , df2, (*avg1H) , df3 ,DI );
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
            
            temp= dht1.readTemperature();

            if (min_temp > temp)
              min_temp=temp;

            if (max_temp < temp)
              max_temp=temp;
            
            hum = dht1.readHumidity();

            if (min_hum > hum)
              min_hum=temp;

            if (max_hum < hum)
              max_hum=hum;
            
            if ((!isnan(temp) && !isnan(hum))) {
              if ((temp >= DHT22_MIN_TEMP && temp <= DHT22_MAX_TEMP) && (hum >= DHT22_MIN_HUM && hum <= DHT22_MAX_HUM)){
                (*avg1C) += temp;
                (*avg1H) += hum;
                ++s1;
              }
              else{
                main_sensor = -1;
                PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR);
//                delay(STANDARD_DELAY_TIME);
//                PRINT_TO_USER(USING_DHT11, LCD_USING_DHT11);
//                delay(STANDARD_DELAY_TIME);
              }
            }else{
              PRINT_TO_USER(UNAVAILABLE_DHT22, LCD_UNAVAILABLE_DHT22);
              main_sensor = -1;
              // main_sensor = 11; // Make DHT11 main sensor (DHT22 not available)
              // delay(STANDARD_DELAY_TIME);
              // PRINT_TO_USER(USING_DHT11, LCD_USING_DHT11);
              delay(STANDARD_DELAY_TIME);
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

          if (min_temp2 > temp2)
              min_temp2=temp;

            if (max_temp2 < temp2)
              max_temp2=temp2;
          
          hum2 = dht1.readHumidity();

          if (min_hum2 > hum2)
              min_hum2=temp2;

            if (max_hum2 < hum2)
              max_hum2=hum2;
          
          if ((!isnan(temp2) && !isnan(hum2))) {
              if ((temp2 >= DHT11_MIN_TEMP && temp2 <= DHT11_MAX_TEMP) && (hum2 >= DHT11_MIN_HUM && hum2 <= DHT11_MAX_HUM)){
                (*avg2C) += temp2;
                (*avg2H) += hum2;
                ++s2;
              }
              else{
                PRINT_TO_USER(OUT_OF_BOUND_ERROR, LCD_OUT_OF_BOUND_ERROR);
                main_sensor = -1;
                delay(STANDARD_DELAY_TIME);
              }
          }else{
            PRINT_TO_USER(UNAVAILABLE_DHT11, LCD_UNAVAILABLE_DHT11);
            main_sensor = -1; // -1 if none of sensors are available
            delay(STANDARD_DELAY_TIME);
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
      DI_temp= round(((*avg1C) - 0.55 * (1 - 0.01 * (*avg1H)) * (*avg1C) - 14.5));

      PRINT_TO_USER(SEPARATOR, LCD_EMPTY);
      PRINT_TO_USER(IN_CONDITIONS, LCD_EMPTY);
      PRINT_TO_USER(TEMPERATURE, LCD_TEMPERATURE);
      print_to_user(String((*avg1C)), String((*avg1C)));
      delay(STANDARD_DELAY_TIME);
//      lcd.clear();
      PRINT_TO_USER(HUMIDITY, LCD_HUMIDITY);
      print_to_user(String((*avg1H)), String((*avg1H)));
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

      PRINT_TO_USER(SEPARATOR, LCD_EMPTY);
      PRINT_TO_USER(IN_CONDITIONS, LCD_EMPTY);
      PRINT_TO_USER(TEMPERATURE, LCD_TEMPERATURE);
      print_to_user(String((*avg2C)), String((*avg2C)));
      delay(STANDARD_DELAY_TIME);
//      lcd.clear();
      PRINT_TO_USER(HUMIDITY, LCD_HUMIDITY);
      print_to_user(String((*avg2H)), String((*avg2H)));
    }
    else{
      PRINT_TO_USER(SENSORS_UNAVAILABLE, LCD_SENSORS_UNAVAILABLE);
      delay(STANDARD_DELAY_TIME);
      PRINT_TO_USER(RESET, LCD_RESET);
      delay(STANDARD_DELAY_TIME);
      return false;
    }

    *DI = calc_DI(DI_temp);
//      write_data_to_server( iot_server, channelID , df1, (*avg1C) , df2, (*avg1H) , df3 ,DI );
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

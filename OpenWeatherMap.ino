/*
 * Function:  get_from_OpenWeatherMapAPI 
 * --------------------
 * gets data from OpenWeatherMap API
 *
 *  temp_out: pointer to temperature outside variable
 *  hum_out: pointer to humidity outside variable
 *
 *  returns: true, get was successful
 *           false, if get wasn't successful
 */

bool get_from_OpenWeatherMapAPI(float * temp_out, float * hum_out){

      String result;

      PRINT_TO_USER(GETTING_DATA, LCD_GETTING_DATA, 0, 0, true, true);
      delay(STANDARD_DELAY_TIME);

      if (client.connect(weather_server, 80))
      {                                              //starts client connection, checks for connection
        client.println("GET /data/2.5/weather?id="+city_id+"&units=metric&APPID="+weather_api_key);
        client.println("Host: "+String(weather_server));
        client.println("User-Agent: ArduinoWiFi/1.1");
        client.println("Connection: close");
        client.println();
      }
      else
      {
        PRINT_TO_USER(API_CONNECTION_FAILED,LCD_API_CONNECTION_FAILED, 0, 1, true, false);  //error message if no client connect
        delay(STANDARD_DELAY_TIME);
        return false;
      }

      while(client.connected() && !client.available())
      delay(1);                                    //waits for data
      while (client.connected() || client.available()) //connected or data available
      {                                            
        char c = client.read();                    //gets byte from ethernet buffer
        result = result+c;
      }

      client.stop();                              //stop client
      result.replace('[', ' ');
      result.replace(']', ' ');
      char jsonArray [result.length()+1];
      result.toCharArray(jsonArray,sizeof(jsonArray));
      jsonArray[result.length() + 1] = '\0';
      StaticJsonBuffer<1024> json_buf;
      JsonObject &root = json_buf.parseObject(jsonArray);

      if (!root.success())
      {
        PRINT_TO_USER(PARSE_OBJECT_FAILED, LCD_PARSE_OBJECT_FAILED, 0, 1, true, false);
        delay(STANDARD_DELAY_TIME);
        return false;
      }

      (*temp_out) = root["main"]["temp"];
      (*hum_out) = root["main"]["humidity"];

      PRINT_TO_USER(OUT_CONDITIONS, LCD_EMPTY, 0, 0, true, true);
      delay(STANDARD_DELAY_TIME);
      PRINT_TO_USER(TEMPERATURE, LCD_TEMPERATURE, 0, 0, true, true);
      delay(STANDARD_DELAY_TIME);
      print_to_user(String((*temp_out)), String((*temp_out)), 10, 0, true, false);
      delay(STANDARD_DELAY_TIME);
      PRINT_TO_USER(HUMIDITY, LCD_HUMIDITY, 0, 1, true, false);
      delay(STANDARD_DELAY_TIME);
      print_to_user(String((*hum_out)), String((*hum_out)), 9, 1, true, false);
      delay(STANDARD_DELAY_TIME);

      return true;                             //Success
}

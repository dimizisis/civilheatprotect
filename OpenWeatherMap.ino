bool get_from_OpenWeatherMapAPI(){                        //client function to send/receive GET request data.

      String result;

      PRINT_TO_USER(GETTING_DATA, LCD_GETTING_DATA);
      delay(STANDARD_DELAY_TIME);

      if (client.connect(weather_server, 80))
      {                                              //starts client connection, checks for connection
        client.println("GET /data/2.5/weather?id="+city_id+"&units=metric&APPID="+weather_api_key);
        client.println("Host: api.openweathermap.org");
        client.println("User-Agent: ArduinoWiFi/1.1");
        client.println("Connection: close");
        client.println();
      }
      else
      {
        PRINT_TO_USER(API_CONNECTION_FAILED,LCD_API_CONNECTION_FAILED);  //error message if no client connect
        return false;
      }

      while(client.connected() && !client.available())
      delay(1);                                    //waits for data
      while (client.connected() || client.available())
      {                                            //connected or data available
        char c = client.read();                    //gets byte from ethernet buffer
        result = result+c;
      }

      client.stop();                              //stop client
      result.replace('[', ' ');
      result.replace(']', ' ');
      //Serial.println(result);
      char jsonArray [result.length()+1];
      result.toCharArray(jsonArray,sizeof(jsonArray));
      jsonArray[result.length() + 1] = '\0';
      StaticJsonBuffer<1024> json_buf;
      JsonObject &root = json_buf.parseObject(jsonArray);

      if (!root.success())
      {
        PRINT_TO_USER(PARSE_OBJECT_FAILED,LCD_PARSE_OBJECT_FAILED);
        return false;
      }

      tempOutside = root["main"]["temp"];
      humOutside = root["main"]["humidity"];

      PRINT_TO_USER(OUT_CONDITIONS, LCD_EMPTY);
      PRINT_TO_USER(TEMPERATURE, LCD_TEMPERATURE);
      print_to_user(String(tempOutside), String(tempOutside));
      delay(STANDARD_DELAY_TIME);
  //      lcd.clear();
      PRINT_TO_USER(HUMIDITY, LCD_HUMIDITY);
      print_to_user(String(humOutside), String(humOutside));

      delay(STANDARD_DELAY_TIME);

      return true;                             //Success
}

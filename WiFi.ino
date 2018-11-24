/*
 * Function:  wifi_connect
 * --------------------
 * connects to WiFi, if available
 *
 *  returns true, if WiFi connection successes
 *          false, if WiFi connection fails
 */

bool wifi_connect(){

  bool connected = false;

  WiFi.begin(String(wifi_ssid).c_str(), String(wifi_pass).c_str());
  Serial.println();
  PRINT_TO_USER(ACCESSING_WIFI, LCD_ACCESSING_WIFI, 0, 0, true, true);

  uint32_t tStart = millis(); // WiFi starts the attempt to connect
  while (1){
    if ((millis()-tStart) >= WIFI_MAX_WAITING_TIME){
      PRINT_TO_USER(WIFI_NOT_CONNECTED, LCD_WIFI_NOT_CONNECTED, 0, 1, true, false);
      break;
    }
    else if (WiFi.status() == WL_CONNECTED){
      connected = true;
      PRINT_TO_USER(WIFI_CONNECTED, LCD_WIFI_CONNECTED, 0, 1, true, false);
      break;
    }
    delay(1000);
    PRINT_TO_USER(DOT, LCD_EMPTY, 0, 0, true, false);
  }
  ThingSpeak.begin(client);
  return connected;
}

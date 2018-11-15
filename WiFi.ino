
bool   wificonnect(){

  bool connected = false;

  WiFi.begin(String(wifi_ssid).c_str(), String(wifi_pass).c_str());
  Serial.println();
  PRINT_TO_USER(ACCESSING_WIFI, LCD_ACCESSING_WIFI, true);

  uint32_t tStart = millis(); // WiFi starts the attempt to connect
  while (1){
    if ((millis()-tStart) >= WIFI_MAX_WAITING_TIME){
      PRINT_TO_USER(WIFI_NOT_CONNECTED, LCD_WIFI_NOT_CONNECTED, true);
      break;
    }
    else if (WiFi.status() == WL_CONNECTED){
      connected = true;
      PRINT_TO_USER(WIFI_CONNECTED, LCD_WIFI_CONNECTED, true);
      break;
    }
    delay(1000);
    PRINT_TO_USER(DOT, LCD_EMPTY, true);
  }
  ThingSpeak.begin( client );
  return connected;
}

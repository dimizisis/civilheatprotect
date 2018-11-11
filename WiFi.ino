
bool   wificonnect(){

  bool connected = false;

  WiFi.begin(String(wifi_ssid).c_str(), String(wifi_pass).c_str());
  Serial.println();
  PRINT_TO_USER(ACCESSING_WIFI, LCD_ACCESSING_WIFI);

  uint32_t tStart = millis(); // WiFi starts the attempt to connect
  while (1){
    if ((millis()-tStart) >= WIFI_MAX_WAITING_TIME){
      PRINT_TO_USER(WIFI_NOT_CONNECTED, LCD_WIFI_NOT_CONNECTED);
      break;
    }
    else if (WiFi.status() == WL_CONNECTED){
      connected = true;
      PRINT_TO_USER(WIFI_CONNECTED, LCD_WIFI_CONNECTED);
      break;
    }
    delay(1000);
    PRINT_TO_USER(DOT, LCD_EMPTY);
  }
//  ThingSpeak.begin( client );
  return connected;
}

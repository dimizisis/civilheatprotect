void get_setting_from_file(IniFile ini, char* section, char* entry){

  /* Gets entry's value from specific section of config file */

  if (ini.getValue(section, entry, buffer, BUFFER_SIZE)) {
    Serial.print("section " + String(section) + " has an entry " + String(entry) +" with value ");
    Serial.println(buffer);
  }
  else {
    Serial.print("Could not read " + String(entry) + " from section " + String(section) +", error was ");
    print_config_file_error(ini.getError());
  }

}

void read_settings_from_sd(IniFile ini){
  
  /* Reading user settings from configuration file */

  get_setting_from_file(ini, USER_SETTINGS, "WIFI_SSID");

  wifi_ssid = String(buffer);

  get_setting_from_file(ini, USER_SETTINGS, "WIFI_PASS");

  wifi_pass = String(buffer);

  get_setting_from_file(ini, USER_SETTINGS, "WRITE_API_KEY");

  writeAPIKey = buffer;

  get_setting_from_file(ini, USER_SETTINGS, "CHANNEL_ID");

  sscanf(buffer, "%ld", &channelID);

  get_setting_from_file(ini, USER_SETTINGS, "CITY_ID");

  city_id = String(buffer);

  /* Reading system settings from configuration file */

  get_setting_from_file(ini, SYSTEM_SETTINGS, "IOT_SERVER");

  iot_server = String(buffer);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "DELAY_TIME");

  sscanf(buffer, "%d", &delay_time);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "TIME_FRAME");

  sscanf(buffer, "%ld", &timeframe);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "READS_PER_FRAME");

  sscanf(buffer, "%d", &readsperframe);

//  get_setting_from_file(ini, SYSTEM_SETTINGS, "MAIN_SENSOR_TYPE");
//
//  main_sensor = get_sensor_num();

  get_setting_from_file(ini, SYSTEM_SETTINGS, "WEATHER_API_KEY");

  weather_api_key = String(buffer);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "WEATHER_API_SERVER_LINK");

  strcpy(weather_server, buffer);
  
}

void get_default_settings(){

  /* Get default settings, if SD fails */

  iot_server = "ThingSpeak";
  channelID = 0;
  writeAPIKey = "0";
  delay_time = 500;
  timeframe = 60000;
  readsperframe = 10;
  main_sensor = 11;

  wifi_ssid="Test";
  wifi_pass="Test";
}

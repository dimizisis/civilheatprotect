/*
 * Function:  get_setting_from_file
 * --------------------
 * gets a specific setting from configuration file (.ini format)
 * 
 * ini: configuration file (.ini format)
 * section: name of section in configuration file
 * entry: name of the entry inside the section
 *
 *  returns nothing, void func
 *  
 */

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

/*
 * Function:  read_settings_from_sd
 * --------------------
 * reads all settings (section, entry) from a configuration file. Uses get_setting_from_file function
 * 
 * ini: configuration file (.ini format)
 *
 *  returns nothing, void func
 *  
 */

void read_settings_from_sd(IniFile ini){
  
  /* Reading system settings from configuration file */

  get_setting_from_file(ini, SYSTEM_SETTINGS, "IOT_SERVER");

  iot_server = String(buffer);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "DELAY_TIME");

  sscanf(buffer, "%d", &delay_time);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "TIME_FRAME");

  sscanf(buffer, "%ld", &time_frame);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "READS_PER_FRAME");

  sscanf(buffer, "%d", &reads_per_frame);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "WEATHER_API_KEY");

  weather_api_key = String(buffer);

  get_setting_from_file(ini, SYSTEM_SETTINGS, "WEATHER_API_SERVER_LINK");

  strcpy(weather_server, buffer);

  /* Reading user settings from configuration file */

  get_setting_from_file(ini, USER_SETTINGS, "WIFI_SSID");

  wifi_ssid = String(buffer);

  get_setting_from_file(ini, USER_SETTINGS, "WIFI_PASS");

  wifi_pass = String(buffer);

  get_setting_from_file(ini, USER_SETTINGS, "CHANNEL_ID");

  sscanf(buffer, "%ld", &channel_id);

  get_setting_from_file(ini, USER_SETTINGS, "CITY_ID");

  city_id = String(buffer);

  get_setting_from_file(ini, USER_SETTINGS, "ZIP_CODE");

  zip_code = String(buffer);

  get_setting_from_file(ini, USER_SETTINGS, "WRITE_API_KEY");

  write_api_key = buffer;

}

/*
 * Function:  get_default_settings
 * --------------------
 * initializes settings variables with default settings (if SD & WiFi fails)
 *
 *  returns nothing, void func
 *  
 */

void get_default_settings(){

  /* Get default settings, if SD fails */

  iot_server = "ThingSpeak";
  channel_id = 0;
  write_api_key = "0";
  delay_time = 500;
  time_frame = 60000;
  reads_per_frame = 10;

  wifi_ssid="Test";
  wifi_pass="Test";
}

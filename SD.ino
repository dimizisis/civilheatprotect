/*
 * Function:  readSD 
 * --------------------
 * reads SD card, if available
 *
 *  returns 0 when everything is OK
 *          1 when SD card failed to initialize
 *          2 when configuration file does not exist
 *          3 when configuration file is not valid
 */

int readSD(){

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  PRINT_TO_USER(INITIALIZING_SD, LCD_INITIALIZING_SD, 0, 0, true, true);
  delay(STANDARD_DELAY_TIME);
  if (!SD.begin(D10)) {
    PRINT_TO_USER(SD_CARD_NOT_INITIALIZED, LCD_SD_CARD_NOT_INITIALIZED, 0, 1, true, false);
    delay(STANDARD_DELAY_TIME);
    return  1;
  }
  PRINT_TO_USER(SD_CARD_INITIALIZED, LCD_SD_CARD_INITIALIZED, 0, 1, true, false);

  delay(STANDARD_DELAY_TIME);

  IniFile ini(CONFIGURATION_FILENAME);

   if (!ini.open()) {
    Serial.println();
    Serial.print("Ini file ");
    Serial.print(CONFIGURATION_FILENAME);
    Serial.println(" does not exist");
    return 2;
  }
  Serial.println("Ini file exists");

  // Check the file is valid. This can be used to warn if any lines
  // are longer than the buffer.
  if (!ini.validate(buffer, BUFFER_SIZE)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid: ");
    print_config_file_error(ini.getError());
    return 3;
  }

  read_settings_from_sd(ini);

  return 0;

}

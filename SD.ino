int readSD(){

  /*
   *  Returns 0 when everything is OK
   *  Returns 1 when SD card failed to initialize
   *  Returns 2 when configuration file does not exist
   *  Returns 3 when configuration file is not valid
   */

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  PRINT_TO_USER(SEPARATOR, LCD_EMPTY);
  PRINT_TO_USER(INITIALIZING_SD, LCD_INITIALIZING_SD);
  if (!SD.begin(4)) {
    PRINT_TO_USER(SD_CARD_NOT_INITIALIZED, LCD_SD_CARD_NOT_INITIALIZED);
    return 1;
  }
  PRINT_TO_USER(SD_CARD_INITIALIZED, LCD_SD_CARD_INITIALIZED);

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

//  myFile = SD.open(CONFIGURATION_FILENAME);
//  if (myFile) {
//    Serial.println("Read contents of ");
//
//    // read from the file until there's nothing else in it:
//    while (myFile.available()) {
//      Serial.write(myFile.read());
//    }
//    // close the file:
//    myFile.close();
//  } else {
//    // if the file didn't open, print an error:
//    Serial.println("error opening ");
//    return false;
//  }
  return true;
}

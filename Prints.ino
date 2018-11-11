void print_to_user(String serial_message, String lcd_message){

  /* Prints messages to serial & arduino's LCD */

  if (serial_message == TEMPERATURE || serial_message == HUMIDITY || serial_message == IN_CONDITIONS || serial_message == OUT_CONDITIONS || serial_message == DOT || serial_message == ACCESSING_WIFI)
    Serial.print(serial_message);
  else
    Serial.println(serial_message);

//  lcd.clear();
//  lcd.print(lcd_message);
}

void print_config_file_error(uint8_t e, bool eol){

  /* Prints exact error when config file fails to open */

  switch (e) {
  case IniFile::errorNoError:
    Serial.print("no error");
    break;
  case IniFile::errorFileNotFound:
    Serial.print("file not found");
    break;
  case IniFile::errorFileNotOpen:
    Serial.print("file not open");
    break;
  case IniFile::errorBufferTooSmall:
    Serial.print("buffer too small");
    break;
  case IniFile::errorSeekError:
    Serial.print("seek error");
    break;
  case IniFile::errorSectionNotFound:
    Serial.print("section not found");
    break;
  case IniFile::errorKeyNotFound:
    Serial.print("key not found");
    break;
  case IniFile::errorEndOfFile:
    Serial.print("end of file");
    break;
  case IniFile::errorUnknownError:
    Serial.print("unknown error");
    break;
  default:
    Serial.print("unknown error value");
    break;
  }
  if (eol)
    Serial.println();
}

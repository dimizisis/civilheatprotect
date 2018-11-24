/*
 * Function:  print_to_user 
 * --------------------
 * prints specific message to user (serial & lcd)
 *
 *  serial_message: the message printed in serial
 *  lcd_message: the message printed in lcd
 *  cursor1: the number of beggining character in lcd (0-15)
 *  cursor2: the line in lcd (0-1)
 *
 *  returns: nothing, void func
 */

void print_to_user(String serial_message, String lcd_message, int cursor1, int cursor2, bool write_to_lcd, bool clear_lcd){

  if (serial_message == TEMPERATURE || serial_message == HUMIDITY || serial_message == IN_CONDITIONS || serial_message == OUT_CONDITIONS || serial_message == DOT || serial_message == ACCESSING_WIFI)
    Serial.print(serial_message);
  else
    Serial.println(serial_message);

  if (write_to_lcd){
    if (clear_lcd)
      lcd.clear();
    lcd.setCursor(cursor1, cursor2);
    lcd.print(lcd_message);
}
}

/*
 * Function:  initialize_lcd
 * --------------------
 * initializes lcd monitor
 *
 *
 *  returns: nothing, void func
 */

void initialize_lcd(){                  
  lcd.init();
  lcd.backlight();
}

/*
 * Function:  print_config_file_error
 * --------------------
 * prints specific error from reading config file
 *
 *  e: error that returned from open() func
 *  eol: end of line
 *  returns: nothing, void func
 */

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

/*
 * Function:  write_to_server 
 * --------------------
 * writes data to IOT server
 *
 *  server: the server we want to send data to
 *  field1Data: data of field 1
 *  field2Data: data of field 2
 *  field3Data: data of field 3
 *  field4Data: data of field 4
 *
 *  returns: 200, if post was successful
 *           -1, if IOT server not supported
 *           other value, if post wasn't successful
 *           
 */

int write_to_server(String server, float field1Data, float field2Data, int field3Data, int field4Data){
  
  if (server == THINGSPEAK_SERVER){
    ThingSpeak.setField(field1, field1Data);
    ThingSpeak.setField(field2, field2Data);
    ThingSpeak.setField(field3, field3Data);
    ThingSpeak.setField(field4, field4Data);

    int write_success;
    if (write_success = ThingSpeak.writeFields(channel_id, write_api_key) != 200)
      print_to_user(DATA_SEND_FAIL, LCD_DATA_SEND_FAIL, 0, 0, true, true);
    
    return write_success;
  }
  print_to_user(IOT_SERV_NOT_SUPPORTED, LCD_IOT_SERV_NOT_SUPPORTED, 0, 0, true, true);
  return -1;
}

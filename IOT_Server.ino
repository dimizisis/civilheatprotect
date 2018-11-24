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
 *           other value, if post wasn't successful
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
    delay(STANDARD_DELAY_TIME);
    
    return write_success;
  }
}

//void write_metadata_to_server (){
//
//  int postCode = 54635;
//
//  String postMessage = ("metadata={\"postcode\":" + postCode + "}" );
//
//  int writeSuccess = writeRaw (channelID, postMessage, write_api_key);
//
//  if ( writeSuccess == 200 )
//    Serial.println("Metadata sent to TS.")
//  else
//    Serial.println("Failed send metadata to TS.");
//}

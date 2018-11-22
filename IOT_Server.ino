int write_to_server( String server, long channel, unsigned int field1, float field1Data, unsigned int field2, float field2Data, unsigned int field3, int field3Data, unsigned int field4, int field4Data ){

  // Use this function if you want multiple fields simultaneously

  if ( server == "ThingSpeak" ){
    ThingSpeak.setField( field1, field1Data );
    ThingSpeak.setField( field2, field2Data );
    ThingSpeak.setField( field3, field3Data );
    ThingSpeak.setField( field4, field4Data );

    int writeSuccess = ThingSpeak.writeFields( channel, writeAPIKey );
    if ( writeSuccess == 200 ){
      Serial.println( "Tmp: " + String(field1Data) + "C Hum: " + String(field2Data) + "% DI: " + String(field3Data) + " written to Thingspeak." );
    }
    else
      Serial.println("Failed send data to TS.");
    return writeSuccess;
  }
}

void write_metadata_to_server (){

  int postCode = 54635;

  String postMessage = ("metadata={\"postcode\":" + postCode + "}" );

  int writeSuccess = writeRaw (channelID, postMessage, writeAPIKey);

  if ( writeSuccess == 200 )
    Serial.println("Metadata sent to TS.")
  else
    Serial.println("Failed send metadata to TS.");
}

void post_bulk_to_IOT_server(char* csvBuffer) {
  
     // CSV format to bulk update

     // example : https://api.thingspeak.com/channels/598750/bulk_update.csv&write_api_key=3WAVULEKX06N4LNM&timeFormat=absolute&updates=2018-11-30T10:26:23-0500,2,3,4,|2018-11-29T10:26:23-0500,6,7,8,|

     // before call do :
     // char[] csvBuffer;
     // strcpy(csvBuffer,"write_api_key="+WriteAPIKey+"&time_format=absolute&updates=2018-11-30T10:26:23-0500,2,3,4,|2018-11-29T10:26:23-0500,6,7,8,|");
     // (updates in this example are dummy, insert data from CSV)
    
     // Compute the data buffer length
     
     String data_length = String(strlen(csvBuffer));
    
    // Close any connection before sending a new request
    client.stop();
    
    // POST data to ThingSpeak
    if (client.connect("api.thingspeak.com", 80)) {
      
        client.println(String("POST /channels/")+channelID+"/bulk_update.csv");
        client.println("Host: api.thingspeak.com");
        client.println("Connection: close");
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.println("Content-Length: "+data_length);
        client.println();
        client.println(csvBuffer);
    }
    else {
        Serial.println("Failed to update ThingSpeak channel");
    }
    delay(1000); // Wait to receive the response
    client.parseFloat();
    String resp = String(client.parseInt());
    Serial.println("server-response: " + resp);
}

void post_to_IOT_server(float temp, float hum, float DI) {
  
  String str_sensor = String(temp);
  String str_sensor2 = String(hum);
  String str_sensor3 = String(DI);;

  String postStr = String("api_key=")+writeAPIKey+"&field1="+str_sensor+"&field2="+str_sensor2+"&field3="+str_sensor3;
  
    // POST data to ThingSpeak
    
    if (client.connect("api.thingspeak.com", 80)) {
    
        client.println("POST /update");
        client.println("Host: api.thingspeak.com");
        client.println("Connection: close");
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.print("Content-Length: ");
        client.println(postStr.length());
        client.println();
        client.print(postStr);
    }
    else {
        Serial.println("Failed to update ThingSpeak channel");
    }
    delay(1000); // Wait to receive the response
    client.parseFloat();
    String resp = String(client.parseInt());
    Serial.println("server-response: " + resp);
}

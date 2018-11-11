int write_data_to_server( String server, long channel, unsigned int field1, float field1Data, unsigned int field2, float field2Data, unsigned int field3, long field3Data ){

  // Use this function if you want multiple fields simultaneously

  if ( server == "ThingSpeak" ){
    ThingSpeak.setField( field1, field1Data );
    ThingSpeak.setField( field2, field2Data );
    ThingSpeak.setField( field3, field3Data );

    int writeSuccess = ThingSpeak.writeFields( channel, writeAPIKey );
    if ( writeSuccess ){
      Serial.println( "Tmp: " + String(field1Data) + "C Hum: " + String(field2Data) + "% DI: " + String(field3Data) + " written to Thingspeak." );
    }
    return writeSuccess;
  }

}

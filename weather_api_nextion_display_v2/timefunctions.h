



String timestampToTimeString( unsigned long epochtime ) {
  //char hours, minutes, seconds;
   // print the hour, minute and second:
    int minutes = ((epochtime % 3600) / 60);
    minutes = minutes ; //Add UTC Time Zone
    
    int hours = (epochtime  % 86400L) / 3600;    
    if(minutes > 59)
    {      
      hours = hours + 1; //Add UTC Time Zone  
      minutes = minutes - 60;
    }
    else
    {
      hours = hours + 1;
    }
    String returnTime = String(hours);
    returnTime += ":";
    
    if ( minutes < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      returnTime += "0";
    }    
    returnTime += minutes;
    
    int seconds = (epochtime % 60);
    if ( seconds < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
    }
     return returnTime;
}

//void updateTime(){
//
//  delay(1000);
//  timeClient.update();
//  epochTime = timeClient.getEpochTime();
//  delay(1000);
//  if ( epochTime > 90000 ) {
//    internetTime = timestampToTimeString( epochTime );
//    Nextzeitaktuell.setText ( internetTime );
//    
//    if ( DEBUG == true ) { 
//      Serial.print( "Time: ");
//      Serial.println(internetTime);
//    }
//  } else {
//    if ( DEBUG == true ) { 
//      Serial.print( "Time fail");
//    }
//    updateTime();
//  }
//}
//
//void updateTimeLocal() {
//  epochTime = epochTime + 60;
//  internetTime = timestampToTimeString( epochTime );
//  if ( DEBUG == true ) { 
//    Serial.print( "Time: ");
//    Serial.println(internetTime);
//  }
//  if ( DEBUG == true ) { 
//    Serial.println("set time in 1 second");
//  }
//  yield();
//  delay( 1000 );
//  Nextzeitaktuell.setText ( "time" );
//  if ( DEBUG == true ) { 
//    Serial.println("time set");
//  }
//  delay( 1000 );
//  dimDisplay();
//}

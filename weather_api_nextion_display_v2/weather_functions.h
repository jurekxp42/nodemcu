
void showHeute( ) {
    if ( DEBUG == true ) {
      Serial.println ( "show heute" );
    }
   
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();

    WiFiClient client;
    HTTPClient http;
    //Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://api.openweathermap.org/data/2.5/weather?id=2867543&APPID=31a78be80554c0f4776c76af58ed50da&lang=de&units=metric")) {  // HTTP
      
      if ( DEBUG == true ) {
        Serial.print("[HTTP] GET...\n");
      }
      // start connection and send HTTP header
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        if ( DEBUG == true ) {
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        }
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String json = http.getString();
          const size_t capacity = JSON_ARRAY_SIZE(3) + 2*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 3*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 340;
          DynamicJsonDocument doc(capacity);
          deserializeJson(doc, json);
          JsonArray weather = doc["weather"];        
          JsonObject weather_0 = weather[0];
          int weather_0_id = weather_0["id"]; // 301
          const char* weather_0_main = weather_0["main"]; // "Drizzle"
          String weather_0_description = weather_0["description"]; // "drizzle"
          const char* weather_0_icon = weather_0["icon"]; // "09n"
          JsonObject main = doc["main"];
          String main_temp = main["temp"]; // 281.87
          String main_feels_like = main["feels_like"]; // 281.87
          String main_pressure = main["pressure"]; // 1032
          String main_humidity = main["humidity"]; // 100
          String wind_speed = doc["wind"]["speed"]; // 1.5

          unsigned long settime = doc["dt"];
          String printTime = timestampToTimeString( settime);
          int hourofTime = hour( settime );
          int hourstogo = 24 - hourofTime  + 12;
          nextweather =  hourstogo / 3 ;   
 
          if ( DEBUG == true ) {
            Serial.println ( json );
          }
          Nexttemp.setText( main_temp+"*C" );
          Nextfeels.setText( main_feels_like+"*C" );
          Nextclouds.setText( weather_0_description );
          Nextpressure.setText ( main_pressure+" hPa" );
          Nexthumidity.setText ( main_humidity+" %" );
          Nextwind.setText ( wind_speed+" m/s" );
          NexttempInnen.setText( String(temperature)+"*C"  );
          NexthumidityInnen.setText( String(humidity)+" %"  );
          Nextzeitaktuell.setText ( printTime );
        }
      } else {
        if ( DEBUG == true ) {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
      }
      http.end();
    } else {
      if ( DEBUG == true ) {
        Serial.printf("[HTTP} Unable to connect\n");
      }
    }

}

void showMorgen() {
  
  OWM_fiveForecast *ow_fcast5 = new OWM_fiveForecast[40];
  byte entries = owF5.updateForecast(ow_fcast5, 40, ow_key, "de", "2867543", "metric");

  int nextweather16 = nextweather + 8;
  int nextweather24 = nextweather + 16;
  
  unsigned long settime8 = ow_fcast5[nextweather].dt.toInt();
  String dayFormat = String( day(settime8) );
  String monthFormat = String( month(settime8) );
  NextvdateMorgen.setText( dayFormat+"."+monthFormat+". "+timestampToTimeString( settime8 ) );
  Nextv1Temp.setText( ow_fcast5[nextweather].temp+" C" );
  Nextv1Clouds.setText( ow_fcast5[nextweather].description );

  unsigned long settime16 = ow_fcast5[nextweather16].dt.toInt();
  String dayFormat16 = String( day(settime16) );
  String monthFormat16 = String( month(settime16) );
  NextvdateUmorgen.setText( dayFormat16+"."+monthFormat16+". "+timestampToTimeString( settime16 ) );
  Nextv2Temp.setText( ow_fcast5[nextweather16].temp+" C" );
  Nextv2Clouds.setText( ow_fcast5[nextweather16].description );

  unsigned long settime24 = ow_fcast5[nextweather24].dt.toInt();
  String dayFormat24 = String( day(settime24) );
  String monthFormat24 = String( month(settime24) );
  NextvdateUUmorgen.setText( dayFormat24+"."+monthFormat24+". "+timestampToTimeString( settime24 ) );
  Nextv3Temp.setText( ow_fcast5[nextweather24].temp+" C" );
  Nextv3Clouds.setText( ow_fcast5[nextweather24].description );
  
  delete[] ow_fcast5;
}

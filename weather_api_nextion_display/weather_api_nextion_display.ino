#define DEBUG true
#define LED D4

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
ESP8266WiFiMulti WiFiMulti;
#include <ArduinoJson.h>
#include "OpenWeatherMap.h"
#include "weather_functions.h"

int DELAYTIME = 1000 * 60 * 1;
OWMfiveForecast    owF5;
const char *ow_key      = "31a78be80554c0f4776c76af58ed50da";

#include "Nextion.h"

void b0PopCallback(void *ptr);
void button_backPopCallback( void *ptr );
//void heutePopCallback(void *ptr);
//void BmorgenPopCallback(void *ptr);
//void uebermPopCallback(void *ptr);

NexText Nexttemp(0, 1, "main_temp");
NexText Nextfeels(0, 1, "feels_like");
NexText Nextclouds(0, 1, "clouds");
NexText Nextpressure(0, 1, "pressure");
NexText Nexthumidity(0, 1, "humidity");
NexText Nextwind(0, 1, "wind");

NexText Nexttemp1(1, 1, "main_temp1");
NexText Nextfeels1(1, 1, "feels_like1");
NexText Nextclouds1(1, 1, "clouds1");
NexText Nextpressure1(1, 1, "pressure1");
NexText Nexthumidity1(1, 1, "humidity1");
NexText Nextwind1(1, 1, "wind1");

NexText Nexttemp2(2, 1, "main_temp2");
NexText Nextfeels2(2, 1, "feels_like2");
NexText Nextclouds2(2, 1, "clouds2");
NexText Nextpressure2(2, 1, "pressure2");
NexText Nexthumidity2(2, 1, "humidity2");
NexText Nextwind2(2, 1, "wind2");

NexButton b0(0, 15, "b0");
NexButton Bmorgen(0, 9, "morgen");
NexButton Buebermorgen(0, 10, "uebermorgen");
NexButton button_back(1, 2, "button_back");

NexPage page0    = NexPage(0, 0, "page0");
NexPage page1    = NexPage(1, 0, "page1");
NexPage page2    = NexPage(2, 0, "page2");


char buffer[100] = {0};
#define LED D4

NexTouch *nex_listen_list[] = 
{
    &b0,
    &button_back,
    &page0,
    &page1,
    &page2,
    NULL
};

//void heutePopCallback(void *ptr)
//{
//    dbSerialPrintln("heutePopCallback");
//    page0.show();
//    showHeute();
//}
//
//void morgenPopCallback(void *ptr)
//{
//    NexButton *btn = (NexButton *)ptr;
//    if ( DEBUG == true ) {
//      Serial.println("morgen");
//    }
//    dbSerialPrintln("morgenPopCallback");
//    page1.show();
//    showMorgen();
//}
//
//void uebermPopCallback(void *ptr)
//{
//    dbSerialPrintln("uebermPopCallback");
//    page2.show();
//    showUebermorgen();
//}


void setup(void)
{
  if ( DEBUG == true ) {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    digitalWrite(LED, LOW);
  }
  for (uint8_t t = 2; t > 0; t--) {
    if ( DEBUG == true ) {
      Serial.printf("[SETUP] WAIT %d...\n", t);
    }
    //Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("MayBeNot", "EmmerichLan");
  digitalWrite(LED, HIGH);
  if ( DEBUG == true ) {
    Serial.print("Wait for WiFi... ");
  }
    while (WiFiMulti.run() != WL_CONNECTED) {
      if ( DEBUG == true ) {
        Serial.print(".");
      }
      delay(500);
    }
  if ( DEBUG == true ) {  
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  
   nexInit();
   b0.attachPop(b0PopCallback);
//     Bheute.attachPop(heutePopCallback, &Bheute);
//     Bmorgen.attachPop(morgenPopCallback, &Bmorgen);
//     Buebermorgen.attachPop(uebermPopCallback, &Buebermorgen);
//     Bheute2.attachPop(heutePopCallback, &Bheute2);
//     Bmorgen2.attachPop(morgenPopCallback, &Bmorgen2);
//     Buebermorgen2.attachPop(uebermPopCallback, &Buebermorgen2);

      
     dimDisplay(40);
     delay(1000);
     dimDisplay(100);
     delay(1000);
     
     dimDisplay(10);
     delay(1000);
     dimDisplay(80);
     delay(1000);
     dimDisplay(0);
     delay(1000);
     dimDisplay(100);
     delay(1000);
    showHeute();
     
    
}
void b0PopCallback(void *ptr)
{
    Serial.println( "show page1" );
    page1.show();
}
void button_backPopCallback(void *ptr)
{
    Serial.println( "show page1" );
    page0.show();
}
void loop(void)
{
   nexLoop(nex_listen_list);
    
}


void dimDisplay( int percent ) {
  Serial.println( "Dimm to :"+String(percent) );
  String dim = "dim=" + String(percent);
  sendCommand(dim.c_str());
  
}


void showHeute() {
    
    WiFiClient client;
    HTTPClient http;
    //Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://api.openweathermap.org/data/2.5/weather?id=2867543&APPID=31a78be80554c0f4776c76af58ed50da&lang=de&units=metric")) {  // HTTP
      digitalWrite(LED, LOW);
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
          
          if ( DEBUG == true ) {
            Serial.println("Sending response: "+main_temp );
            Serial.println ( json );
          }
          digitalWrite(LED, LOW);
          //String messageSend = main_temp+";"+main_feels_like+";"+String(main_pressure)+";"+String(main_humidity);
          Nexttemp.setText( main_temp+" °C" );
          Nextfeels.setText( main_feels_like+" °C" );
          Nextclouds.setText( weather_0_description );
          Nextpressure.setText ( main_pressure+" hPa" );
          Nexthumidity.setText ( main_humidity+" %" );
          Nextwind.setText ( wind_speed+" m/s" );
          delay(500);
          digitalWrite(LED, HIGH);
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
  Serial.print("Entries: "); Serial.println(entries+1);
  for (byte i = 0; i <= entries; ++i) { 
    if ( i == 8 ) {
      digitalWrite(LED, LOW);
      Nexttemp1.setText( ow_fcast5[8].temp+" °C" );
      Nextfeels1.setText( ow_fcast5[8].feels_like+" °C" );
      Nextclouds1.setText( ow_fcast5[8].description  );
      Nextpressure1.setText ( ow_fcast5[8].pressure+" hPa" );
      Nexthumidity1.setText ( ow_fcast5[8].humidity+" %" );
      Nextwind1.setText ( ow_fcast5[8].wind_speed+" m/s" );
      delay(500);
      digitalWrite(LED, HIGH);
      Serial.print(ow_fcast5[i].dt + ": icon: ");
      Serial.print(ow_fcast5[i].icon + ", temp.: [" + ow_fcast5[i].t_min + ", " + ow_fcast5[i].t_max + "], press.: " + ow_fcast5[i].pressure);
      Serial.println(", descr.: " + ow_fcast5[i].description + ":: " + ow_fcast5[i].cond + " " + ow_fcast5[i].cond_value);
    }
  }
  delete[] ow_fcast5;
}

void showUebermorgen() {
    
    OWM_fiveForecast *ow_fcast5 = new OWM_fiveForecast[40];
  byte entries = owF5.updateForecast(ow_fcast5, 40, ow_key, "de", "2867543", "metric");
  Serial.print("Entries: "); Serial.println(entries+1);
  for (byte i = 0; i <= entries; ++i) { 
    if ( i == 16 ) {
      digitalWrite(LED, LOW);
      Nexttemp1.setText( ow_fcast5[8].temp+" °C" );
      Nextfeels1.setText( ow_fcast5[8].feels_like+" °C" );
      Nextclouds1.setText( ow_fcast5[8].description  );
      Nextpressure1.setText ( ow_fcast5[8].pressure+" hPa" );
      Nexthumidity1.setText ( ow_fcast5[8].humidity+" %" );
      Nextwind1.setText ( ow_fcast5[8].wind_speed+" m/s" );
      delay(500);
      digitalWrite(LED, HIGH);
      Serial.print(ow_fcast5[i].dt + ": icon: ");
      Serial.print(ow_fcast5[i].icon + ", temp.: [" + ow_fcast5[i].t_min + ", " + ow_fcast5[i].t_max + "], press.: " + ow_fcast5[i].pressure);
      Serial.println(", descr.: " + ow_fcast5[i].description + ":: " + ow_fcast5[i].cond + " " + ow_fcast5[i].cond_value);
    }
  }
  delete[] ow_fcast5;
  
}

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
ESP8266WiFiMulti WiFiMulti;


#include <ArduinoJson.h>

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

#include <GxEPD.h>
#include <GxGDEH029A1/GxGDEH029A1.h>      // 2.9" b/w

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeSans9pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, /*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=D4*/ 2, /*BUSY=D2*/ 4); // default selection of D4(=2), D2(=4)

#define DEBUG true
#define LED D4
int DELAYTIME = 1000 * 60 * 1;

void setup()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  if ( DEBUG == true ) {
    Serial.begin(115200);
  }
  
  
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("MayBeNot", "EmmerichLan");
  digitalWrite(LED, HIGH);
  if ( DEBUG == true ) {
    Serial.print("Wait for WiFi... ");
    while (WiFiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  
  //display.init(115200); // enable diagnostic output on Serial
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont( &FreeSans9pt7b );
  display.update();
  digitalWrite(LED, LOW);
}

void loop()
{
  
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
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

          float coord_lon = doc["coord"]["lon"]; // -0.13
          float coord_lat = doc["coord"]["lat"]; // 51.51
          
          JsonArray weather = doc["weather"];
          
          JsonObject weather_0 = weather[0];
          int weather_0_id = weather_0["id"]; // 301
          const char* weather_0_main = weather_0["main"]; // "Drizzle"
          const char* weather_0_description = weather_0["description"]; // "drizzle"
          const char* weather_0_icon = weather_0["icon"]; // "09n"
          
          JsonObject weather_1 = weather[1];
          int weather_1_id = weather_1["id"]; // 701
          const char* weather_1_main = weather_1["main"]; // "Mist"
          const char* weather_1_description = weather_1["description"]; // "mist"
          const char* weather_1_icon = weather_1["icon"]; // "50n"
          
          JsonObject weather_2 = weather[2];
          int weather_2_id = weather_2["id"]; // 741
          const char* weather_2_main = weather_2["main"]; // "Fog"
          const char* weather_2_description = weather_2["description"]; // "fog"
          const char* weather_2_icon = weather_2["icon"]; // "50n"
          
          const char* base = doc["base"]; // "stations"
          
          JsonObject main = doc["main"];
          String main_temp = main["temp"]; // 281.87
          String main_feels_like = main["feels_like"]; // 281.87
          int main_pressure = main["pressure"]; // 1032
          int main_humidity = main["humidity"]; // 100
          float main_temp_min = main["temp_min"]; // 281.15
          float main_temp_max = main["temp_max"]; // 283.15
          
          int visibility = doc["visibility"]; // 2900
          
          float wind_speed = doc["wind"]["speed"]; // 1.5
          
          int clouds_all = doc["clouds"]["all"]; // 90
          if ( DEBUG == true ) {
            Serial.println ( weather_0_description );
            Serial.println ( main_temp );
            Serial.println(json);
          }
          String firstmessage   = "Temp:     ";
          firstmessage        += main_temp;
          firstmessage        += " C";
          String secondmessage  = "gefuehlt: ";
          secondmessage += main_feels_like;
          secondmessage += " C";
          String thirdmessage = "Druck: ";
          thirdmessage += main_pressure;
          thirdmessage += "    LF: ";
          thirdmessage += main_humidity;
          showMessage ( firstmessage , 0 );
          showMessage ( secondmessage , 20  );
          showMessage ( weather_0_description , 40);
          showMessage ( thirdmessage , 60  );
          display.update();
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
  if ( DEBUG == true ) {
    for ( int i = DELAYTIME ; i > 0 ; i = i - 1000 ) {
      digitalWrite(LED, LOW);
      Serial.println( i );
      delay ( 500 );
      digitalWrite(LED, HIGH);
      delay( 500 );
    }
  } else {
    delay ( DELAYTIME );
  }
  //delay(1200000);
}

void showMessage( String message , int row  ) {

  display.setCursor( 0 , row);
  display.println();
  display.println ( message );
  
}

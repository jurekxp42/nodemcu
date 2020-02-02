#define DEBUG false

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
ESP8266WiFiMulti WiFiMulti;
#include <ArduinoJson.h>
#include "OpenWeatherMap.h"

#include <Time.h>
#include <Ticker.h>
Ticker ticktock1;
Ticker ticktock2;

//Ticker ticktock_dimm;
#include "DHTesp.h"
DHTesp dht;


#include <RCSwitch.h>
RCSwitch sender = RCSwitch();
const char* anschalten = "0F0F0F0FFF0F";
const char* ausschalten= "0F0F0F0FFFF0";

OWMfiveForecast    owF5;
OWMconditions      owCC;

const char *ow_key      = "31a78be80554c0f4776c76af58ed50da";

int nextweather = 0;
#include "Nextion.h"
#include "nextion_config.h"
//
//#include <WiFiUdp.h>
//WiFiUDP ntpUDP;
//#include <NTPClient.h>
//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 60000);
//String internetTime;
//unsigned long epochTime = 0;

String dimlow = "dim=10";
String dimhigh = "dim=100";

void setup(void)
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("MayBeNot", "EmmerichLan");
  //WiFiMulti.addAP("gockelnetz", "BerndAnita1303");
  
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

  sender.enableTransmit(D6); //Pin D3
  sender.setProtocol(1);
  sender.setPulseLength(187);
  
  String thisBoard= ARDUINO_BOARD;
  dht.setup(D5, DHTesp::DHT22);
  
   nexInit();
   b00.attachPop(b00PopCallback);
   b01.attachPop(b01PopCallback);
   b10.attachPop(b10PopCallback);
   b20.attachPop(b20PopCallback);
   b21.attachPop(b21PopCallback);
   b22.attachPop(b22PopCallback);
   showDefault();

   ticktock1.attach(1800, showDefault);
   //ticktock_hourly.attach(3600, updateTime);
   //ticktock_quarter.attach(900, showDefault);
   ticktock2.attach(60, dimDisplay);
}

void loop(void)
{

  nexLoop(nex_listen_list);
    
}


#include "timefunctions.h"
#include "weather_functions.h"

void b00PopCallback(void *ptr)
{
    dimDisplayFull();
    if ( DEBUG == true ) {
      Serial.println( "show page1" );
    }
    page1.show();
    showMorgen();
}
void b01PopCallback(void *ptr)
{
    dimDisplayFull();
    if ( DEBUG == true ) {
      Serial.println( "show page2" );
    }
    page2.show();
    
}
void b10PopCallback(void *ptr)
{
    dimDisplayFull();
    if ( DEBUG == true ) {
      Serial.println( "show page0" );
    }
    page0.show();
    showHeute( );
}
void b20PopCallback(void *ptr)
{
    if ( DEBUG == true ) {
      Serial.println( "licht an" );
    }
    sender.sendTriState( anschalten );
 
}
void b21PopCallback(void *ptr)
{
    if ( DEBUG == true ) {
      Serial.println( "licht aus" );
    }
    sender.sendTriState( ausschalten );
   
}
void b22PopCallback(void *ptr)
{
    dimDisplayFull();
    if ( DEBUG == true ) { 
      Serial.println( "show page0" );
    }
    page0.show();
    showHeute( );
}
void showDefault(){
  Serial.println ( "showDefault" );
   
  page0.show();
  showHeute( );
  delay( 2000 );
  
}


void dimDisplay(  ) {
  sendCommand(dimlow.c_str());
}
void dimDisplayFull(  ) {
  sendCommand(dimhigh.c_str());
}

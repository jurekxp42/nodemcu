

#include <NTPClient.h>
#include <ESP8266WiFi.h> // provides ESP8266 specific Wi-Fi routines we are calling to connect to network
#include <WiFiUdp.h> //handles sending and receiving of UDP packages



const char *ssid     = "MayBeNot";
const char *password = "EmmerichLan";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800,60000);
String arr_days[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
String date_time;
// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
  

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  delay(3000);
  timeClient.begin();
}

void loop() {
  timeClient.update();
  
  Serial.println(timeClient.getFormattedTime());
  
  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int ss = timeClient.getSeconds();
  
  if(hh>12)
  {
    hh=hh-12;
    Serial.print(hh);
    Serial.print(":");
    Serial.print(mm);
    Serial.print(":");
    Serial.print(ss);
    Serial.println(" PM");
  }
  else
  {
    Serial.print(hh);
    Serial.print(":");
    Serial.print(mm);
    Serial.print(":");
    Serial.print(ss);
    Serial.println(" AM");   
  }

  int day = timeClient.getDay();
  Serial.println("'"+arr_days[day]+"'");
  
  date_time = timeClient.getFormattedDate();
  int index_date = date_time.indexOf("T");
  String date = date_time.substring(0, index_date);
  Serial.println(date);
  
}

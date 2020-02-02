#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#include <RCSwitch.h>
RCSwitch sender = RCSwitch();

//anschalten: 1118559
//ausschalten: 1118548
const char* anschalten = "0F0F0F0FFF0F";
const char* ausschalten= "0F0F0F0FFFF0";


// Replace with your network credentials
const char* ssid = "MayBeNot";
const char* password = "EmmerichLan";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

String page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
void setup(void){
  
  delay(1000);
  //Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  //Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());

  sender.enableTransmit(D6); //Pin D6
  sender.setProtocol(1);
  sender.setPulseLength(187);

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);// initialize with the I2C addr 0x3C
  
  server.on("/", [](){
    server.send(200, "text/html", page);
  });
  server.on("/LEDOn", [](){
    server.send(200, "text/html", page);
    //Serial.println( "an" );
    sender.sendTriState( anschalten );
    delay(1000);
  });
  server.on("/LEDOff", [](){
    server.send(200, "text/html", page);
    //Serial.println( "aus" );
    sender.sendTriState( ausschalten );
    delay(1000); 
  });
  server.begin();
  //Serial.println("Web server started!");

}
 
void loop(void){
  server.handleClient();
}

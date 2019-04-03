#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
SSD1306Wire  display(0x3c, D3, D5);

#include <ESP8266WiFi.h>

const char* ssid     = "MayBeNot";
const char* password = "EmmerichLan";
int point = 0;
//128 x 64

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  display.clear();
  
  display.drawString(0, 0 , "Connecting to ");
  display.drawString(0, 10 , ssid );
  display.display();
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.drawString( point , 20 , ".");
    display.display();
    point += 2;
  }

  display.drawString(0, 30 , "WiFi connected");
  display.drawString(0, 40 , "IP address:");
  String ip_address = String(WiFi.localIP());
  display.drawString(0, 50 , ip_address);
  display.display();
  delay(3000);
  
}

int write_text( String string , int x , int y ) {

  display.clear();
  display.drawString(x, y , string);
  display.display();

  return 0;
  
}

void loop() {
  write_text( "hello" , 0 , 0 );
  delay(1000);
  write_text( "world" , 0 , 20 );
  delay( 1000 );
}

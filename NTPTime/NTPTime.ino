/**The MIT License (MIT)

Copyright (c) 2016 by Seokjin Seo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

See more at http://usemodj.com
*/

#include <ESP8266WiFi.h>
#include "NTPTimeClient.h"

// WIFI
const char* WIFI_SSID = "MayBeNot"; 
const char* WIFI_PWD = "EmmerichLan";

// TimeClient settings
const float UTC_OFFSET = 9;

NTPTimeClient timeClient(UTC_OFFSET);

void updateData();

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PWD);
  Serial.println("Connecting to WiFi");
 
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    counter++;
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  updateData();
  delay(10000); //10 sec
}

void updateData(){
  Serial.println("Updating time...");
  timeClient.updateTime();
  Serial.println(timeClient.getFormattedDate("."));
  Serial.println(timeClient.getFormattedTime());
}

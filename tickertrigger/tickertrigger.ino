#include <Ticker.h>

Ticker ticktock;
Ticker ticktock2;

int count = 1;

void updateTime() {
    count++;
    Serial.println ( String(count) );
}

void setup() {
  
  Serial.begin( 115200 );
  // flip the pin every 0.3s
  ticktock.attach(10, updateTime);
  ticktock2.attach(3, something);
}

void loop() {
}
void something() {
  Serial.println ( "do something");
  
}

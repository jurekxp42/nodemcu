
#include <Ticker.h>

Ticker distance_ticker;

#include "botfunctions.h"

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  
  
  delay(3000);
  //distance_ticker.attach(0.3, func_distances );
  func_distances();
}



void loop() {
//testTwo();
}

void testTwo() {

// turn on motors

digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);

// accelerate from zero to maximum speed

for (int i = 0; i < 256; i++)

{
    analogWrite(ENA, i);
    delay(50);
}

}

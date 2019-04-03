#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);// initialize with the I2C addr 0x3C
}

void displayTempHumid(){
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = 50;
  // Read temperature as Celsius
  float t = 20;
  // Read temperature as Fahrenheit
  float f = 25;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    display.clearDisplay(); // clearing the display
    display.setTextColor(WHITE); //setting the color
    display.setTextSize(1); //set the font size
    display.setCursor(5,0); //set the cursor coordinates
    display.print("Failed to read from DHT sensor!");
    return;
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Humidity: "); 
  display.print(h);
  display.print(" %\t");
  display.setCursor(0,10);
  display.print("Temperature: "); 
  display.print(t);
  display.print(" C"); 
  display.setCursor(0,20);
  display.print("Temperature: "); 
  display.print(f);
  display.print(" F"); 
}
void loop() {
  displayTempHumid();
  display.display();
}

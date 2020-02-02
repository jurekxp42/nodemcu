    /////////////////////////////////////////////////////////////////
   //             ESP32 E-PAPER THERMOMETER             v1.00     //
  //       Get the latest version of the code here:              //
 //        http://educ8s.tv/esp32-e-paper-thermometer           //
/////////////////////////////////////////////////////////////////

#include <GxEPD.h>
#include <GxGDEH029A1/GxGDEH029A1.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>


#include <Fonts/FreeSansBold24pt7b.h>


int tempC = 0;

GxIO_Class io(SPI, /*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=D4*/ 2, /*BUSY=D2*/ 4); // default selection of D4(=2), D2(=4)

void setup() {
  
  Serial.begin(9600);
  display.init();
  
}

void loop() {
  
  
  tempC++;
  
  showPartialUpdate(tempC);
  printTemperatureToSerial();
  
  delay(5000);
}

void showPartialUpdate(float temperature)
{
  String temperatureString = String(temperature,1);
  const char* name = "FreeSansBold24pt7b";
  const GFXfont* f = &FreeSansBold24pt7b;
  
  uint16_t box_x = 60;
  uint16_t box_y = 60;
  uint16_t box_w = 90;
  uint16_t box_h = 100;
  uint16_t cursor_y = box_y + 16;

  display.setRotation(45);
  display.setFont(f);
  display.setTextColor(GxEPD_BLACK);

  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(box_x, cursor_y+38);
  display.print(temperatureString); 
  display.updateWindow(box_x, box_y, box_w, box_h, true);
}

void printTemperatureToSerial()
{
  Serial.print(tempC);
  Serial.print(" C");
  Serial.print("\t");
}

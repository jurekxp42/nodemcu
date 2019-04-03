#include <ESP8266WiFi.h>
#include <WebSocketClient.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
#define OUTPUT_READABLE_ACCELGYRO
char send_message = '  ';


const char* ssid     = "MayBeNot";
const char* password = "EmmerichLan";
char path[] = "/";
char host[] = "192.168.18.22";
  
WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(5000);
  
Serial.println( "connect to websocket" );
Serial.println ( host );
  // Connect to the websocket server
  if (client.connect( host , 9000)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    while(1) {
      // Hang on failure
    }
  }

  // Handshake with the server
//  webSocketClient.path = path;
//  webSocketClient.host = host;
//  if (webSocketClient.handshake(client)) {
//    Serial.println("Handshake successful");
//  } else {
//    Serial.println("Handshake failed.");
//    while(1) {
//      // Hang on failure
//    }  
//  }

  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}


void loop() {
  String data;

  if (client.connected()) {
    
//    webSocketClient.getData(data);
//    if (data.length() > 0) {
//      Serial.print("Received data: ");
//      Serial.println(data);
//    }
//    
//    // capture the value of analog 1, send it along
//    pinMode(1, INPUT);
//    data = String(analogRead(1));
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    Serial.print(ax); Serial.print("\t");
    Serial.println(ay);
    
    if( ax < -2000 ) {
      send_message = 'bb';
    }
    if( ax > 2000 ) {
      send_message = 'fw';
    }
    if( ay < -2000 ) {
      send_message = 'll';
    }
    if( ay > 2000 ) {
      send_message = 'rr';
    }
    Serial.println(send_message); 
    webSocketClient.sendData( String(send_message) );
    
  } else {
    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }
  
  // wait to fully let the client disconnect
  delay(3000);
  
}

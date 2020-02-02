#include <RCSwitch.h>
//RCSwitch rcSwitch = RCSwitch();
RCSwitch sender = RCSwitch();

//anschalten: 1118559
//ausschalten: 1118548
//anschalten: 0F0F0F0FFF0F
//ausschalten: 0F0F0F0FFFF0

int sensor_on  = 11; 
int sensor_off = 10;
int statechange  = 0;
int state;
int state2;
int DEBUG = 1;

void setup() {
  if ( DEBUG == 1 ) {
    Serial.begin( 9600 );
  }
  pinMode(sensor_on, INPUT);
  pinMode(sensor_off, INPUT);
//  rcSwitch.enableReceive(0);
  sender.enableTransmit(9); //Pin D3
  sender.setProtocol(1);
  sender.setPulseLength(187);
  //delay( 60000 );
}

void loop() {

//  if (rcSwitch.available()) {
//    Serial.println("<--- Beginn");
//    output(rcSwitch.getReceivedValue(), rcSwitch.getReceivedBitlength(), rcSwitch.getReceivedDelay(), rcSwitch.getReceivedRawdata(),rcSwitch.getReceivedProtocol());
//    rcSwitch.resetAvailable();
//  Serial.println("<--- End");
//  }
  //sender.sendTriState("0F0F0F0FFF0F");
  
  state = digitalRead(sensor_on);
  state2 = digitalRead(sensor_off);
  if ( DEBUG == 1 ) {
    Serial.print( state );
    Serial.print( state2 );
    Serial.print( " | " );
  }
  if ( state2 == HIGH && statechange == 1 ) {
    statechange = 0;
    if ( DEBUG == 1 ) {
      Serial.println("aus");
    //} else {
      sender.sendTriState("0F0F0F0FFFF0");
      delay( 30000 );
    }
  }
  if( state == HIGH && statechange == 0) {
    statechange = 1;
    if ( DEBUG == 1 ) {
      Serial.println("lights on!");
    //} else {
      sender.sendTriState("0F0F0F0FFF0F");
      delay( 3000 );
    }
  }
  
  
  delay( 1000 );

  
}

static const char* bin2tristate(const char* bin);
static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength);
void output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol) {
  const char* b = dec2binWzerofill(decimal, length);
  Serial.print("Decimal: ");
  Serial.print(decimal);
  Serial.print(" (");
  Serial.print(length);
  Serial.println(" Bit)");
  Serial.print("Binary: ");
  Serial.println(b);
  Serial.print("Tri-State: ");
  Serial.println(bin2tristate( b));
  Serial.print("PulseLength: ");
  Serial.print(delay);
  Serial.println(" microseconds");
  Serial.print("Protocol: ");
  Serial.println(protocol);
  
  Serial.println("Raw data: ");
  Serial.print("\t");
  for (unsigned int i=0; i<= length*2; i++) {
  Serial.print(raw[i]);
  Serial.print(",");
  if(i!= 0 && (i%5)==0){
  Serial.println();
  Serial.print("\t");
  }
  }
  Serial.println();
}
static const char* bin2tristate(const char* bin) {
  static char returnValue[50];
  int pos = 0;
  int pos2 = 0;
  while (bin[pos]!='\0' && bin[pos+1]!='\0') {
  if (bin[pos]=='0' && bin[pos+1]=='0') {
  returnValue[pos2] = '0';
  } else if (bin[pos]=='1' && bin[pos+1]=='1') {
  returnValue[pos2] = '1';
  } else if (bin[pos]=='0' && bin[pos+1]=='1') {
  returnValue[pos2] = 'F';
  } else {
  return "not applicable";
  }
  pos = pos+2;
  pos2++;
  }
  returnValue[pos2] = '\0';
  return returnValue;
}
static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength) {
  static char bin[64]; 
  unsigned int i=0;
  while (Dec > 0) {
  bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
  Dec = Dec >> 1;
  }
  for (unsigned int j = 0; j< bitLength; j++) {
  if (j >= bitLength - i) {
  bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
  } else {
  bin[j] = '0';
  }
  }
  bin[bitLength] = '\0';
  
  return bin;
}

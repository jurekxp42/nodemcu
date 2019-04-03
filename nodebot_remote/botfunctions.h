int ENA = D0; //4;
int IN1 = D1; //0;
int IN2 = D2; //2;
int IN3 = D3;
int IN4 = D4;
int ENB = D5;

int trigPin2 = 13;
int echoPin2 = 12;
int trigPin = 3;
int echoPin = 15;

int duration;
int distance;
int dist;
int duration2;
int distance2;
int dist2;

int speedleft = 650;
int speeright = 840;

int func_distance1() {
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  return distance;
    
}
int func_distance2() {
  digitalWrite(trigPin2, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1;
  return distance2;
    
}

void stopit () {
  Serial.println( "stopit" );
  digitalWrite ( ENA , LOW );
  digitalWrite ( ENB , LOW );
  digitalWrite ( IN1 , LOW );
  digitalWrite ( IN2 , LOW );
  digitalWrite ( IN3 , LOW );
  digitalWrite ( IN4 , LOW );

}

void forward () {
  analogWrite( ENA , speedleft );
  analogWrite( ENB , speeright );
  digitalWrite ( IN1 , HIGH );
  digitalWrite ( IN2 , LOW );
  digitalWrite ( IN3 , LOW );
  digitalWrite ( IN4 , HIGH );

}



void backward () {
  analogWrite( ENA , speedleft );
  analogWrite( ENB , speeright );
  digitalWrite ( IN1 , LOW );
  digitalWrite ( IN2 , HIGH );
  digitalWrite ( IN3 , HIGH );
  digitalWrite ( IN4 , LOW );
  
}

void turnaroundleft() {
  //delay(1000);
  //stopit();
  Serial.println( "turn left" );
  analogWrite( ENA , speedleft );
  digitalWrite( ENB , LOW );
  digitalWrite ( IN1 , LOW );
  digitalWrite ( IN2 , HIGH );
  digitalWrite ( IN3 , LOW );
  digitalWrite ( IN4 , LOW );
  delay(650);
  stopit();
  
}

void turnaroundright() {
  
  digitalWrite( ENA , LOW );
  analogWrite( ENB , speeright );
  digitalWrite ( IN1 , LOW );
  digitalWrite ( IN2 , LOW );
  digitalWrite ( IN3 , HIGH );
  digitalWrite ( IN4 , LOW );
  delay(650);
  stopit();
  
}



void func_distances() {
  dist = func_distance1();
  dist2 = func_distance2();
  Serial.print( dist );
  Serial.print( " | " );
  Serial.println( dist2 );
  if ( dist < 10 ) {
    
    Serial.println( "stop dist" );
    stopit();
    delay(100);
    backward();
    delay(200);
    
    turnaroundleft();
    delay(200);
    forward();
    delay( 100 );
    func_distances();
    
    //forward();
  } else if ( dist2 < 10 ) {
    
    Serial.println( "stop dist2" );
    stopit();
    delay(100);
    forward();
    //delay(100);
    //turnaroundright();
    
    //forward();
    delay( 100 );
    func_distances();
  } else {
    delay( 50 );
    func_distances();
    
  }
  
}

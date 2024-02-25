#include <Arduino.h>
#include <ESP32Servo.h>


Servo myservo;
Servo myservo2;


int position = 0;
int ServoPin = 4;
int ServoPin2 = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myservo.setPeriodHertz(50);
  myservo.attach(ServoPin,500,2400);
  myservo2.setPeriodHertz(50);  
  myservo2.attach(ServoPin2,500,2400);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  

  position = 90;
  myservo.write(position);
  Serial.println(myservo.read());

  myservo2.write(position);
}


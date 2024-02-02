#include <Arduino.h>
#include <ESP32Servo.h>


Servo myservo;

int position = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myservo.setPeriodHertz(50);
  myservo.attach(5,500,2400);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(3000);

  position = 0;
  myservo.write(position);
  Serial.println(myservo.read());

  delay(3000);

  position = 90;
  myservo.write(position);
  Serial.println(myservo.read());

  delay(3000);

  position = 180;
  myservo.write(position);
  Serial.println(myservo.read());
}


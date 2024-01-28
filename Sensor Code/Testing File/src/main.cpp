#include <Arduino.h>

int LED = 5;


void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);
  delay(5000);
  digitalWrite(LED, LOW);
  delay(5000);
}

#include <Arduino.h>

int pwmChannel = 0;
int frequency = 50;
int resolution = 8;
int pwmPin = 5;
int MAX_DUTY_CYCLE = (int)(pow(2, resolution-1));


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ledcSetup(pwmChannel, frequency, resolution);
  ledcAttachPin(pwmPin, pwmChannel);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(3000);


  Serial.println("Center");
  ledcWrite(pwmChannel, MAX_DUTY_CYCLE*.15);
  Serial.println(ledcRead(pwmChannel));

  delay(3000);

  Serial.println("Right");
  ledcWrite(pwmChannel, MAX_DUTY_CYCLE*.05);
  Serial.println(ledcRead(pwmChannel));

  delay(3000);

  Serial.println("Left");
  ledcWrite(pwmChannel, MAX_DUTY_CYCLE*.25);
  Serial.println(ledcRead(pwmChannel));
}


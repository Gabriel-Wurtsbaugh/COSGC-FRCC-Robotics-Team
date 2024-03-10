
#include <Arduino.h>
#include <NewPing.h>

#define TRIGGER_PIN 13
#define ECHO_PIN 14
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  delay(2000);
  unsigned int uS = sonar.ping();
  if (uS == 0)
  {
    Serial.println("ping out of max range");
  }
  else
  {
    Serial.print("uS: ");
    Serial.println(uS);
    pinMode(ECHO_PIN, OUTPUT);
    digitalWrite(ECHO_PIN, LOW);
    pinMode(ECHO_PIN, INPUT);
    Serial.print("Ping: ");
    Serial.print(uS / US_ROUNDTRIP_CM); // US_ROUNDTRIP_CM def in library as 57 (microsec takes to travel 1cm)
    Serial.println("cm");
  }
}
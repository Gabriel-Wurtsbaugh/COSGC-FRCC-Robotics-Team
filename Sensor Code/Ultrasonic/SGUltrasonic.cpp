
#include <Arduino.h>
#include <NewPing.h>
#include "NostromoPinsAndGlobals.h"

#define MAX_DISTANCE 200


/*If the rover is too close to something this function returns 1, if it's far enough away it returns 0 */

int is_to_close()
{
    delay(200);
    unsigned int uS = sonar.ping();
    Serial.print("uS: ");
    Serial.println(uS);
    pinMode(ECHO_PIN, OUTPUT);
    digitalWrite(ECHO_PIN, LOW);
    pinMode(ECHO_PIN, INPUT);
    Serial.print("Ping: ");
    unsigned int converted_uS = uS / US_ROUNDTRIP_CM;
    Serial.print(converted_uS); // US_ROUNDTRIP_CM def in library as 57 (microsec takes to travel 1cm)
    Serial.println("cm");
    if (converted_uS < STOP_DISTANCE)
    {
        return 1;   // TOO CLOSE
    }
    else
    {
        return 0;  // FAR ENOUGH
    }
}

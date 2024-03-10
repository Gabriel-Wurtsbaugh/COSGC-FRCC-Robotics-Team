//basic driving code that I will integrate with sensors shortly, still troubleshooting my circuit 

//these make it so visual studio code doesn't give me 5 billion errors 
#include <Arduino.h>
void Forward(int Duration, int pwm_A, int pwm_B);
void Reverse(int Duration, int pwm_A, int pwm_B);
void Stop(int Duration);
void RotateRight(int Duration, int pwm_A, int pwm_B);
void RotateLeft(int Duration, int pwm_A, int pwm_B);

//Side A (right)
int ENA_PIN = 27; //speed
int IN1_PIN = 12; //direction
int IN2_PIN = 14; //direction

//Side B (left)
int ENB_PIN = 35; //speed
int IN3_PIN = 33; //direction
int IN4_PIN = 32; //direction

//time of flight
int SDA_PIN = 21;
int SCL_PIN = 22;

//make it go straight
int straight_A = 255;
int straight_B = 255;

void setup() {
// initialize pins as outputs
pinMode(ENA_PIN, OUTPUT);
pinMode(IN1_PIN, OUTPUT);
pinMode(IN2_PIN, OUTPUT);
pinMode(ENB_PIN, OUTPUT);
pinMode(IN3_PIN, OUTPUT);
pinMode(IN4_PIN, OUTPUT);
//hiiiii helloooooo
Serial.begin(115200);
}

//loop for testing
void loop () {
Serial.println("Vroom vroom");
Forward(4000, straight_A, straight_B);
Stop(1300);
Reverse(4000, straight_A, straight_B);
Stop(1300);
RotateLeft(4000, straight_A, straight_B);
Stop(1300);
RotateRight(4000, straight_A, straight_B);
Serial.println("Hopefully I did the thing");
Stop(5000);

}

/* copy pasted this bad boy
void loop () {
  IRsensorValue = analogRead(IRsensorPin);
  IRsensorVoltage = 5.0*(IRsensorValue/1023.);

  if (IRsensorVoltage < ObjectDetected)
  {
    Serial.print("IR Voltage is ");
    Serial.println(IRsensorVoltage);
    Serial.println("No object detected so driving forward.");
    Forward(4000)
  }
else
{
   Serial.print("IR Voltage is ");
   Serial.println(IRsensorVoltage);
   Serial.println("Object detected so turning.");
   Stop(2000);
   RotateRight(1300)

}
}
*/ 

void Forward(int Duration, int pwm_A, int pwm_B)
{
//Function to drive forward for amount of time Duration

Serial.print("Forward for ");
Serial.print(Duration/1000);
Serial.println(" seconds");
//Side A spins clockwise
digitalWrite(IN1_PIN, HIGH); 
digitalWrite(IN2_PIN, LOW); 
//Side B spins clockwise
digitalWrite(IN3_PIN, HIGH);
digitalWrite(IN4_PIN, LOW);
//pwm yippee
analogWrite(ENA_PIN, pwm_A);
analogWrite(ENB_PIN, pwm_B);
delay(Duration);

}

void Reverse(int Duration, int pwm_A, int pwm_B)
{
//Function to drive reverse for amount of time Duration

Serial.print("Reverse for ");
Serial.print(Duration/1000);
Serial.println(" seconds");
//Side A spins counterclockwise
digitalWrite(IN1_PIN, LOW); 
digitalWrite(IN2_PIN, HIGH); 
//Side B spins counterclockwise
digitalWrite(IN3_PIN, LOW);
digitalWrite(IN4_PIN, HIGH);
//pwm yippee
analogWrite(ENA_PIN, pwm_A);
analogWrite(ENB_PIN, pwm_B);
delay(Duration);

}

void Stop(int Duration)
{

Serial.print("Stop for ");
Serial.print(Duration/1000);
Serial.println(" seconds");
analogWrite(ENA_PIN, 0);
analogWrite(ENB_PIN, 0);
delay(Duration);

}

void RotateRight(int Duration, int pwm_A, int pwm_B)
{
//Function to drive right rotation for amount of time Duration

Serial.print("RotateRight for ");
Serial.print(Duration/1000);
Serial.println(" seconds");
//Side A spins counterclockwise
digitalWrite(IN1_PIN, LOW); 
digitalWrite(IN2_PIN, HIGH); 
//Side B spins clockwise
digitalWrite(IN3_PIN, HIGH);
digitalWrite(IN4_PIN, LOW);
//pwm yippee
analogWrite(ENA_PIN, pwm_A);
analogWrite(ENB_PIN, pwm_B);
delay(Duration);

}

void RotateLeft(int Duration, int pwm_A, int pwm_B)
{
//Function to drive left rotation for amount of time Duration

Serial.print("RotateLeft for ");
Serial.print(Duration/1000);
Serial.println(" seconds");
//Side A spins clockwise
digitalWrite(IN1_PIN, HIGH); 
digitalWrite(IN2_PIN, LOW); 
//Side B spins counterclockwise
digitalWrite(IN3_PIN, LOW);
digitalWrite(IN4_PIN, HIGH);
//pwm yippee
analogWrite(ENA_PIN, pwm_A);
analogWrite(ENB_PIN, pwm_B);
delay(Duration);

}

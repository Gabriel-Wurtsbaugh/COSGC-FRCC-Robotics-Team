//Side A (right)
int ENA_PIN = 27;//speed
int IN1_PIN = 12; //direction
int IN2_PIN = 14; //direction

//Side B (left)
int ENB_PIN = 35; //speed
int IN3_PIN = 33; //direction
int IN4_PIN = 32; //direction

//make it go straight. idk what the biggest value is so i'm just putting 255 for now
//will require testing, making these so values are easy to adjust
int straight_A = 255;
int straight_B = 255;

//times for testing
int TimeDefault = 4000;
int Time90 = 1300;

void setup() {
// initialize pins as outputs
pinMode(ENA_PIN, OUTPUT);
pinMode(IN1_PIN, OUTPUT);
pinMode(IN2_PIN, OUTPUT);
pinMode(ENB_PIN, OUTPUT);
pinMode(IN3_PIN, OUTPUT);
pinMode(IN4_PIN, OUTPUT);
}

//loop for testing
void loop () {
Serial.println("Preparing to move");
Forward(TimeDefault, straight_A, straight_B);
Stop(Time90);
Reverse(TimeDefault, straight_A, straight_B);
Stop(Time90);

}

void Forward(int Duration, int pulsewidth_A, int pulsewidth_B)
{
//Function to drive forward for amount of time Duration, which is argument passed
//to function. Drives wheels using parameters pulsewidth_a and pulsewidth_b.

Serial.print("Forward for ");
Serial.print(Duration/1000);
Serial.println(" seconds");
//Side A spins clockwise
digitalWrite(IN1_PIN, HIGH); 
digitalWrite(IN2_PIN, LOW); 
//Side B spins clockwise
digitalWrite(IN3_PIN, HIGH);
digitalWrite(IN4_PIN, LOW);
//idek honestly something something go forward
analogWrite(ENA_PIN, pulsewidth_A);
analogWrite(ENB_PIN, pulsewidth_B);
delay(Duration);

}

void Reverse(int Duration, int pulsewidth_A, int pulsewidth_B)
{
//Function to drive reverse for amount of time Duration, which is argument passed
//to function. Drives wheels using parameters pulsewidth_a and pulsewidth_b.

Serial.print("Reverse for ");
Serial.print(Duration/1000);
Serial.println(" seconds");
//Side A spins counterclockwise
digitalWrite(IN1_PIN, LOW); 
digitalWrite(IN2_PIN, HIGH); 
//Side B spins counterclockwise
digitalWrite(IN3_PIN, LOW);
digitalWrite(IN4_PIN, HIGH);
//screams into the darkness
analogWrite(ENA_PIN, pulsewidth_A);
analogWrite(ENB_PIN, pulsewidth_B);
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

void RotateRight(int Duration, int pulsewidth_A, int pulsewidth_B)
{
//Function to drive right rotation for amount of time Duration, which is argument passed
//to function. Drives wheels using parameters pulsewidth_a and pulsewidth_b.

Serial.print("RotateRight for ");
Serial.print(Duration/1000);
Serial.println(" seconds");
//Side A spins counterclockwise
digitalWrite(IN1_PIN, LOW); 
digitalWrite(IN2_PIN, HIGH); 
//Side B spins clockwise
digitalWrite(IN3_PIN, HIGH);
digitalWrite(IN4_PIN, LOW);
//screams into the darkness
analogWrite(ENA_PIN, pulsewidth_A);
analogWrite(ENB_PIN, pulsewidth_B);
delay(Duration);

}

void RotateLeft(int Duration, int pulsewidth_A, int pulsewidth_B)
{
//Function to drive right rotation for amount of time Duration, which is argument passed
//to function. Drives wheels using parameters pulsewidth_a and pulsewidth_b.

Serial.print("RotateLeft for ");
Serial.print(Duration/1000);
Serial.println(" seconds");
//Side A spins clockwise
digitalWrite(IN1_PIN, HIGH); 
digitalWrite(IN2_PIN, LOW); 
//Side B spins counterclockwise
digitalWrite(IN3_PIN, LOW);
digitalWrite(IN4_PIN, HIGH);
//screams into the darkness
analogWrite(ENA_PIN, pulsewidth_A);
analogWrite(ENB_PIN, pulsewidth_B);
delay(Duration);

}

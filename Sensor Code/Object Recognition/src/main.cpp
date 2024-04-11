#include <vector>
#include <Arduino.h>
#include <SparkFun_VL53L5CX_Library.h>
#include <Wire.h>
#include <cmath>
#include <ESP32Servo.h>
using namespace std;



//Data Structures
SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData; // Result data class structure, 1356 bytes of RAM, Used to store distances
Servo rotationalServo; //Creates servo objects
Servo pitchServo;

//ToF Servo Assembly Variables
int rotServoPin = 4;
int pitchServoPin = 5;


//Universal Variables
int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output
int minDistance = 150; //Set nearest distance an object can get to the sensor (mm)
int height1 =128; //Know height of the ToF sensor off the ground on a flat plane (mm)

int zone1Locations[24] = {0,1,2,8,9,10,16,17,18,24,25,26,32,33,34,40,41,42,48,49,50,56,57,58};
int zone2Locations[16] = {3,4,11,12,19,20,27,28,35,36,43,44,51,52,59,60};
int zone3Locations[24] = {5,6,7,13,14,15,21,22,23,29,30,31,37,38,39,45,46,47,53,54,55,61,62,63};

int zone1Values[24] = {};
int zone2Values[16] = {};
int zone3Values[24] = {};


// put function declarations here:
void sensorIntitiation();
bool dataProcessed();
int objectAvoidance(int* direction);
bool objectDetection();
void remindTheSensorItExists();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //Runs sensor intiation function
  sensorIntitiation();

  //Sets up servo objects to be used in the rest of the code
  rotationalServo.setPeriodHertz(50);
  
  pitchServo.setPeriodHertz(50);
  rotationalServo.attach(rotServoPin,500,2400);
  pitchServo.attach(pitchServoPin,500,2400);

  rotationalServo.write(90);
  pitchServo.write(90);

  Serial.println("Set Servos to 90");
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);


  //Variables
  int direction;
  bool detection;

  //Prime sensor to gather good data
  remindTheSensorItExists();

  //Detect if the sensor has data that can be collected and checks the data for any objects within range. 
  if (dataProcessed() == true) 
  {
    Serial.println("Data Ready!");

    delay(1000);

    //Runs objectDetection algorithm
    Serial.println("Running Object detection");
    objectAvoidance(&direction);

  }

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------


// put function definitions here:
//Function to intitiat ToF Sensor Settings so I can Clean my code a bit and make it easier to copy and paste
void sensorIntitiation() {
/*
|
|
Intializes the VL53L5CXm Time of flight sensor so that it is prepared to start requesting data from
|
|
*/

  delay(1000);
  Serial.println("SparkFun VL53L5CX Imager Example");

  Wire.begin(); //This resets to 100kHz I2C
  Wire.setClock(400000); //Sensor has max I2C freq of 400kHz

  Serial.println("Initializing sensor board. This can take up to 10s. Please wait.");
  if (myImager.begin() == false)
  {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1) ;
  }

  myImager.setResolution(8*8); //Enable all 64 pads

  imageResolution = myImager.getResolution(); //Query sensor for current resolution - either 4x4 or 8x8
  imageWidth = sqrt(imageResolution); //Calculate printing width

  //The sensor starts collecting data
  myImager.startRanging();
}

//checks to see if data can be collected from sensor and if true reads the data into an array
bool dataProcessed() {
//Checks to see if the TOF sensor has data prepared to be outputed
  if (myImager.isDataReady() == true)
  {
    if (myImager.getRangingData(&measurementData)) //Read distance data into array
    {
      return true;
    }
  }
  return false;
}

bool objectDetection()
{
  for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
  {
    for (int x = imageWidth - 1 ; x >= 0 ; x--)
    {
      //Rejects the values from all of the corner zones when doing general object detection
      switch (x+y)
      {
      case 56:
      break;
      case 57:
      break;
      case 48:
      break;
      case 62:
      break;
      case 63:
      break;
      case 55:
      break;
      case 15:
      break;
      case 7:
      break;
      case 6:
      break;
      case 8:
      break;
      case 0:
      break;
      case 1:
      break;
      default:
        if (measurementData.distance_mm[x+y] < 300)
        {
        return true;
        }         
        break;
      }
    }
  }
}

//Checks to see if an object has come within the min detect distance
int objectAvoidance(int* direction) {

  //Sets direction to undecided(0)
  *direction = 0;

  //Logic for zone 1 & 3
  for(int i = 0; i <= 23; ++i)
  {
    int x = measurementData.distance_mm[zone1Locations[i]];
    int y = measurementData.distance_mm[zone3Locations[i]];

    if (x < minDistance)
    {
      zone1Values[i] = 1;
    } else{zone1Values[i] = 0;}

    if (y < minDistance)
    {
      zone3Values[i] = 1;
    } else{zone3Values[i] = 0;}
  }

  //Logic for zone 2
  for(int i = 0; i <= 15; ++i)
  {
    int z = measurementData.distance_mm[zone2Locations[i]];

    if (z < minDistance)
    {
      zone2Values[i] = 1;
    } else{zone2Values[i] = 0;}
  } 

  //Logic to calculate percentages
  int zone1Total = 0;
  int zone2Total = 0;
  int zone3Total = 0;

  for(int i = 0; i <= 23; ++i)
  {
    zone1Total = zone1Total + zone1Values[i];
    zone3Total = zone3Total + zone3Values[i];
  }

  for(int i = 0; i <= 15;)
  {
    zone2Total = zone2Total = zone2Values[i];
  }

  //Logic get percetages and compare them
  int z1P = (zone1Total/24)*100;
  int z2P = (zone2Total/16)*100;
  int z3P = (zone3Total/24)*100;

  if (z1P > z2P && z3P)
  {
    //Set direction to left
    *direction = 1;
  }
  else if (z2P> z1P && z3P)
  {
    //Set direction to forward
    *direction = 2;
  } 
  else if (z3P> z1P && z2P)
  {
    //Set direction to right
    *direction = 3;
  }

  return;
} 


//Flip the sensor upwards to see a close neutral surface to help reset the zone values before taking data
void remindTheSensorItExists() {
  delay(100);

  //Cycle Servo up and dwn 3 times
  for(int i = 0; i <= 3; ++i)
  {
    pitchServo.write(90);
    delay(200);
    pitchServo.write(0);
    delay(200);
  }

  //set back to neutral
  pitchServo.write(90);
}
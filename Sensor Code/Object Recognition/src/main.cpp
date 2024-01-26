#include <Arduino.h>
#include <SparkFun_VL53L5CX_Library.h>
#include <Wire.h>
#include <cmath>

//Data Structures
SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData; // Result data class structure, 1356 bytes of RAM, Used to store distances

int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output
int minDistance = 300;//Set nearest distance an object can get to the sensor (mm)

// put function declarations here:
void sensorIntitiation();
bool dataProcessed();
int objectDetection();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  sensorIntitiation();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);

  //Detect if the sensor has data that can be collected and checks the data for any objects within range. 
  if (dataProcessed() == true) 
  {
    if (objectDetection() == true)
    {
      {
        






      }
    }
  }

}

// put function definitions here:
//Function to intitiat ToF Sensor Settings so I can Clean my code a bit and make it easier to copy and paste
void sensorIntitiation() {
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
  if (myImager.isDataReady() == true)
  {
    if (myImager.getRangingData(&measurementData)) //Read distance data into array
    {
      return true;
    }
  }
  return false;
}

//Checks to see if an object has come within the min detect distance
int objectDetection() {
  //Variables
  bool detection = false;
  int x = 0; //Object as tall or taller than rover if x=1, shorter if x=0


  for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
  {
    for (int x = imageWidth-1; x >= 0; x--)
    {
      if (measurementData.distance_mm[x+y] < minDistance)
      {
        detection = true; //returns true if there is an object within minimum distance.
      } else {return false;} //quits out the function if no object is detected
    }

    if (detection == true)
    {
      //Variables 
      int array[4] = {(35,36,27,28)}; 
      int ave = 0;

      //Sums the values for the distances read by the center four zones on the sensor.
      for (int i = 0; i <= 3; ++i)
      {
        ave = ave + measurementData.distance_mm[array[i]];
      }

      //Divides by four to get the average
      ave = ave/4;

      //Checks to see if  the center of the sensor can see the object
      if (ave <= minDistance)
      {
        x = 1;
      }
    }
  }
  return detection, x;
}

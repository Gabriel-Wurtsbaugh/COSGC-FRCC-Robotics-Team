//Code to find distance ratios to calibrate a plane for our sensor to look at.

#include <Arduino.h>
#include <SparkFun_VL53L5CX_Library.h>
#include <Wire.h>

//Data Structures
SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData; // Result data class structure, 1356 bytes of RAM, Used to store distances

int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output

// put function declarations here:
void sensorIntitiation();
bool dataProcessed(); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  sensorIntitiation(); //Intiate ToF Sensor
}

void loop() {
  //Pulls data from sensor and reads it into an array
  if (dataProcessed() == true)
  {
      //Variables
      int topEdge = 0; //Average value of combined zones 3 and 4
      int center = 0;  //Average value of combined zones 35, 36, 27, 28.
      int angle = 0; //Angle

      //The values for the zones that we'll need to access to do the calculation
      int zoneArray [6] = {3 , 4, 27, 28, 35, 36};

      //Gets top edge value
      for (int i = 0; i <= 1; ++i)
      {
        //Sum distance Values at the wanted indexs saved in zone array
        topEdge = topEdge + measurementData.distance_mm[zoneArray[i]];
      }
      //Divides by two to get average value
      topEdge = topEdge/2;

      for (int i = 2; i<= 5; ++i)
      {
        //Sum distance Values at the wanted indexs saved in zone array
        center = center + measurementData.distance_mm[zoneArray[i]];
      }
      //Divides by 5 to get average value
      center = center/5;

      //Prints values to serial
      Serial.print("topEdge = ");
      Serial.println(topEdge);

      Serial.print("center = ");
      Serial.println(center);

      //Math to calculate angle to check calibration
      angle = acos(center/topEdge);

      Serial.print("Angle = ");
      Serial.println(angle);
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


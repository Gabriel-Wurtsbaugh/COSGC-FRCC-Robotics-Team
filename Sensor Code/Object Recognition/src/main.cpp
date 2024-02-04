#include <Arduino.h>
#include <SparkFun_VL53L5CX_Library.h>
#include <Wire.h>
#include <cmath>
#include <ESP32Servo.h>



//Data Structures
SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData; // Result data class structure, 1356 bytes of RAM, Used to store distances
Servo rotationalServo; //Creates servo objects
Servo pitchServo;

//ToF Servo Assembly Variables
int rotServoPin = 0;
int pitchServoPin = 18;


//Universal Variables
int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output
int minDistance = 300; //Set nearest distance an object can get to the sensor (mm)
int height1 = 100; //Know height of the ToF sensor off the ground on a flat plane (mm)


// put function declarations here:
void sensorIntitiation();
bool dataProcessed();
int objectDetection(int* smol, bool* detection);
int heightCalculator(int smol);
int getDistance();


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
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);


  //Variables
  int smol;
  bool detection;


  //Detect if the sensor has data that can be collected and checks the data for any objects within range. 
  if (dataProcessed() == true) 
  {
    //Runs objectDetection algorithm
    objectDetection(&smol, &detection);

    if (detection == true)
    {
      heightCalculator(smol);
    }
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


//Checks to see if an object has come within the min detect distance
int objectDetection(int* smol, bool* detection) {
/*
Detects if an object is within a set minimum distance.
Please not that the set distance is not a flat plane and is isntead curved

After an object is detected it checks to see if the object is within minimum distance right in front of it
to check if the object is taller than or shorter than the rover.
*/
  //Variables
  *detection = false;
  *smol = 0; //Object as tall or taller than rover if x=1, shorter if x=0


  for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
  {
    for (int x = imageWidth-1; x >= 0; x--)
    {
      if (measurementData.distance_mm[x+y] < minDistance)
      {
        *detection = true; //returns true if there is an object within minimum distance.
      } else {return 0;} //quits out the function if no object is detected
    }

    if (*detection == true)
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
        *smol = 1;
      }
    }else {return 0;}
  }
  return 0;
} 

int getDistance() {

  //Variables
  int array[4] = {(35,36,27,28)}; //Sensor indexes for center 4 regions
  int x = 0;

  for (int i = 0; i <= 3; ++i)
    {
      x = x + measurementData.distance_mm[array[i]];
    }

  x = x/4; //Gets the average

  return x;
}



int heightCalculator(int smol) {
/*
|
|
Calculates the height of a detected object and outputs the heights
|
|
*/


  //All of the variable nesscary for height calculations
  //All values in mm
  //Distance Variables
  int distance1 = 0;
  int distance2 = 0;
  int distance3 = 0;

  //Height Variables
  int height2 = 0;
  int height3 = 0;
  int heightTotal = 0;

  //Angle Variables
  int theta1 = 0;
  int theta2 = 0;
  
  //Measures the height of an object taller than the wrover.-----------------------------------------------
  if (smol == 1)
  {
    //Gets distance for D1
    distance1 = getDistance(); 

    //Calculates theta 1
    theta1 = atan(height1/distance1); 

    //Moves the pitch Servo to theta one of the x-axis
    pitchServo.write(90 - theta1); 

    distance2 = getDistance();

    pitchServo.write(90); //Move servo back to neutral

    //Variables needed to find top of object
    int current = 0;
    int previous = distance1;

    //Finds the top of the object
    for(int i = 90; i <= 180; ++i)
      {
        delay(250);
        int current = getDistance();

        //Checks to see if a change in distance was more than 10 cm
        if ((current-previous) < 100)
        {
          previous = current;
        }
        else 
        {
          //Found the top so read previous values
          theta2 = pitchServo.read()-1;
          distance3 = previous;
          break;
        }

        //Moves servo to next increment
        pitchServo.write(i);
      }

    height3 = distance3*sin(theta2);

    heightTotal = height2 + height3;
  } //End of taller object height calculation.
  else
  {
    pitchServo.write(45); //Move servo back to neutral

    //Variables needed to find top of object
    int current = 0;
    int previous = getDistance();
    
    //Finds the top of the object
    for(int i = 45; i <= 90; ++i)
    {
      delay(250);
      int current = getDistance();

      //Checks to see if a change in distance was more than 10 cm
      if ((current-previous) < 100)
      {
        previous = current;
      }
      else 
      {
        //Found the top so read previous values
        theta1 = pitchServo.read()-1;
        distance1 = previous;
        break;
      }

      //Moves servo to next increment
      pitchServo.write(i);
    }

    distance2 = distance1*cos(theta1);

    theta2 = atan(height1/distance2);

    pitchServo.write(90-theta2);

    distance3 = getDistance();

    height2 = distance2*tan(theta2);

    height3 = distance1*sin(theta1);

    heightTotal = height2-height3;
  }

  return heightTotal;
}
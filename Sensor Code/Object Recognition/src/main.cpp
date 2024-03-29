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
vector<int> detectedObjectVector = {};
vector<int> correspondingDistanceVector = {};

//ToF Servo Assembly Variables
int rotServoPin = 4;
int pitchServoPin = 5;


//Universal Variables
int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output
int minDistance = 150; //Set nearest distance an object can get to the sensor (mm)
int height1 =128; //Know height of the ToF sensor off the ground on a flat plane (mm)


// put function declarations here:
void sensorIntitiation();
bool dataProcessed();
int objectDetection(int* smol, bool* detection);
int heightCalculator(int smol);
int getDistance();
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
  int smol;
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
    objectDetection(&smol, &detection);

    if (detection == true)
    {
      int x = heightCalculator(smol);

      Serial.println("Height= ");
      Serial.println(x);
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
        Serial.println("Object Detected!");

        *detection = true; //returns true if there is an object within minimum distance.
        
        detectedObjectVector.push_back(x+y);
        correspondingDistanceVector.push_back(measurementData.distance_mm[x+y]);

        //Output snap shot of the sensor when an object is detected
        if (myImager.isDataReady() == true)
        {
          if (myImager.getRangingData(&measurementData)) //Read distance data into array
          {
            //The ST library returns the data transposed from zone mapping shown in datasheet
            //Pretty-print data with increasing y, decreasing x to reflect reality
            for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
            {
        
              for (int x = imageWidth - 1 ; x >= 0 ; x--)
              {
                Serial.print("\t");
                Serial.print(measurementData.distance_mm[x + y]);
              }
              Serial.println();
            }
            Serial.println();
          }
        }

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
        Serial.println("Smol= ");
        Serial.print(*smol);
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

  Serial.print("Distance =");
  Serial.println(x);

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
    Serial.println("Taller");   
    //Gets distance for D1
    distance1 = getDistance(); 

    Serial.print("Distance1= ");
    Serial.println(distance1);
    delay(500);

    //Calculates theta 1
    theta1 = atan(height1/distance1);
    Serial.print("theta1= ");
    Serial.println(theta1);
    delay(500); 

    //Moves the pitch Servo to theta one of the x-axis
    pitchServo.write(90 - theta1); 

    distance2 = getDistance();
    Serial.print("distance2= ");
    Serial.println(distance2);
    delay(500); 


    pitchServo.write(90); //Move servo back to neutral

    //Variables needed to find top of object
    int current = 0;
    int previous = distance1;

    //Finds the top of the object
    for(int i = 0; i <= 90; ++i)
      {
//Snapshot of sensor values
if (myImager.isDataReady() == true)
  {
    if (myImager.getRangingData(&measurementData)) //Read distance data into array
    {
      //The ST library returns the data transposed from zone mapping shown in datasheet
      //Pretty-print data with increasing y, decreasing x to reflect reality
      for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
      {
        
        for (int x = imageWidth - 1 ; x >= 0 ; x--)
        {
          Serial.print("\t");
          Serial.print(measurementData.distance_mm[x + y]);
        }
        Serial.println();
      }
      Serial.println();
    }
  }
    
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
          Serial.print("distance3= ");
          Serial.println(distance3);
          delay(500); 

          
          break;
        }

        //Moves servo to next increment
        Serial.println("Increment Servo....");
        delay(1000);
        pitchServo.write(i);
        Serial.println(i);
      }

    height3 = distance3*sin(theta2);
    Serial.print("Height3= ");
    Serial.println(height3);
    delay(500); 

    
    heightTotal = height2 + height3;
    Serial.print("heightTotal= ");
    Serial.println(heightTotal);
    delay(500); 

  } //End of taller object height calculation.
  else //Find height of shorter object
  {
Serial.println("Shorter");

    pitchServo.write(45); //Move servo back to neutral

    //Variables needed to find top of object
    int current = 0;
    int previous = getDistance();
    
    //Finds the top of the object
    for(int i = 90; i <= 135; ++i)
    {
//Snapshot of sensor values
if (myImager.isDataReady() == true)
  {
    if (myImager.getRangingData(&measurementData)) //Read distance data into array
    {
      //The ST library returns the data transposed from zone mapping shown in datasheet
      //Pretty-print data with increasing y, decreasing x to reflect reality
      for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
      {
        
        for (int x = imageWidth - 1 ; x >= 0 ; x--)
        {
          Serial.print("\t");
          Serial.print(measurementData.distance_mm[x + y]);
        }
        Serial.println();
      }
      Serial.println();
    }
  }
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
      Serial.println("Increment Servo....");
      delay(1000);
      pitchServo.write(i);
      Serial.println(i);
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

//Flip the sensor upwards to see a close neutral surface to help reset the zone values before taking data
void remindTheSensorItExists() {
  delay(100);

  //Cycle Servo up and dwn 3 times
  for(int i = 0; i <= 3; ++i)
  {
    pitchServo.write(90);
    delay(500);
    pitchServo.write(0);
    delay(500);
  }

  //set back to neutral
  pitchServo.write(90);
}
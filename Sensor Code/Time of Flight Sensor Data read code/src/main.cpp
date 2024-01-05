/*
This is the adapted code from the documentation that outputs and arrary of distances in mm.

I wasn't sure how to read or get values from the array it was producing so I went ahead and wrote somecode to make a "Monitoring Array" so that we know where our data zone on the sensor are.

While they match the order in the documentation they are reflected on both the y and x axis. This allows us to unmirror the image that the sensor is seeing / the zones in the documentation.
*/

#include <Arduino.h>
#include <SparkFun_VL53L5CX_Library.h>
#include <Wire.h>

SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData; // Result data class structure, 1356 bytes of RAM

int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output

void setup()
{
  Serial.begin(115200);
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

void loop()
{
  //Process to write an array into Serial Monitor so that you can see what the sensor is seeing--------------------------------------------------
  //Poll sensor for new data
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

/**/
      //So that I can understand what the call values in the array look like
      Serial.println("Monitoring Array");
      for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
      {
        //Start new row
        Serial.print("Y Val: ");
        Serial.println(y);
        Serial.println();

        for (int x = imageWidth - 1 ; x >= 0 ; x--)
        {
          Serial.print("\t");
          Serial.print(x+y);
        }
        Serial.println();
      }
      Serial.println();
    
  
  /**/
  //End of Array Writing Process
  delay(5); //Small delay between polling
}

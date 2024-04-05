#include <Arduino.h>

#include <Wire.h>

#include <Adafruit_LIS3MDL.h>

#include <Adafruit_Sensor.h>



Adafruit_LIS3MDL lis3mdl;

int counter = 0;



// function declarations

void cardinalDir(float headingDegrees);



void setup(void)

{

  Serial.begin(9600);

  while (!Serial)

    delay(10); // will pause Zero, Leonardo, etc until serial console opens



  delay(5000);

  Serial.println("Adafruit LIS3MDL test!");



  // Try to initialize!

  if (!lis3mdl.begin_I2C())

  { // hardware I2C mode, can pass in address & alt Wire

    // if (! lis3mdl.begin_SPI(LIS3MDL_CS)) {  // hardware SPI mode

    // if (! lis3mdl.begin_SPI(LIS3MDL_CS, LIS3MDL_CLK, LIS3MDL_MISO, LIS3MDL_MOSI)) { // soft SPI

    Serial.println("Failed to find LIS3MDL chip");

    while (1)

    {

      delay(10);

    }

  }

  Serial.println("LIS3MDL Found!");



  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);

  Serial.print("Performance mode set to: ");

  switch (lis3mdl.getPerformanceMode())

  {

  case LIS3MDL_LOWPOWERMODE:

    Serial.println("Low");

    break;

  case LIS3MDL_MEDIUMMODE:

    Serial.println("Medium");

    break;

  case LIS3MDL_HIGHMODE:

    Serial.println("High");

    break;

  case LIS3MDL_ULTRAHIGHMODE:

    Serial.println("Ultra-High");

    break;

  }



  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);

  Serial.print("Operation mode set to: ");

  // Single shot mode will complete conversion and go into power down

  switch (lis3mdl.getOperationMode())

  {

  case LIS3MDL_CONTINUOUSMODE:

    Serial.println("Continuous");

    break;

  case LIS3MDL_SINGLEMODE:

    Serial.println("Single mode");

    break;

  case LIS3MDL_POWERDOWNMODE:

    Serial.println("Power-down");

    break;

  }



  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);

  // You can check the datarate by looking at the frequency of the DRDY pin

  /*

  Serial.print("Data rate set to: ");

  switch (lis3mdl.getDataRate())

  {

  case LIS3MDL_DATARATE_0_625_HZ:

    Serial.println("0.625 Hz");

    break;

  case LIS3MDL_DATARATE_1_25_HZ:

    Serial.println("1.25 Hz");

    break;

  case LIS3MDL_DATARATE_2_5_HZ:

    Serial.println("2.5 Hz");

    break;

  case LIS3MDL_DATARATE_5_HZ:

    Serial.println("5 Hz");

    break;

  case LIS3MDL_DATARATE_10_HZ:

    Serial.println("10 Hz");

    break;

  case LIS3MDL_DATARATE_20_HZ:

    Serial.println("20 Hz");

    break;

  case LIS3MDL_DATARATE_40_HZ:

    Serial.println("40 Hz");

    break;

  case LIS3MDL_DATARATE_80_HZ:

    Serial.println("80 Hz");

    break;

  case LIS3MDL_DATARATE_155_HZ:

    Serial.println("155 Hz");

    break;

  case LIS3MDL_DATARATE_300_HZ:

    Serial.println("300 Hz");

    break;

  case LIS3MDL_DATARATE_560_HZ:

    Serial.println("560 Hz");

    break;

  case LIS3MDL_DATARATE_1000_HZ:

    Serial.println("1000 Hz");

    break;

  }

  */



  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);

  /*

  Serial.print("Range set to: ");

  switch (lis3mdl.getRange())

  {

  case LIS3MDL_RANGE_4_GAUSS:

    Serial.println("+-4 gauss");

    break;

  case LIS3MDL_RANGE_8_GAUSS:

    Serial.println("+-8 gauss");

    break;

  case LIS3MDL_RANGE_12_GAUSS:

    Serial.println("+-12 gauss");

    break;

  case LIS3MDL_RANGE_16_GAUSS:

    Serial.println("+-16 gauss");

    break;

  }

  */



  lis3mdl.setIntThreshold(500);

  lis3mdl.configInterrupt(false, false, true, // enable z axis

                          true,               // polarity

                          false,              // don't latch

                          true);              // enabled!

}



void loop()

{

  Serial.print("\ntest #: ");

  Serial.print(counter);

  counter = counter + 1;



  /* Or....get a new sensor event, normalized to uTesla */

  sensors_event_t event;

  lis3mdl.getEvent(&event);



  float headingRadians = atan2(event.magnetic.y, event.magnetic.x);

  float headingDeg = headingRadians * 180 / PI;

  // westminster magnetic declination is 7.6666666667

  // sand dunes (and tinkermill) is 7.75

  // Strasburg is 7.28333333

  // Brighton is 7.566666667

  float declinationAngle = 7.283333333;

  headingDeg += declinationAngle;

  Serial.print(" real headingDegrees");

  Serial.println(headingDeg);



  if (headingDeg < 0)

  {

    headingDeg += 360;

  }



  Serial.print("Heading: ");

  Serial.print(headingDeg);

  cardinalDir(headingDeg);



  /* Display the results (magnetic field is measured in uTesla)

  Serial.print("\tX: ");

  Serial.print(event.magnetic.x);

  Serial.print(" \tY: ");

  Serial.print(event.magnetic.y);

  Serial.print(" \tZ: ");

  Serial.print(event.magnetic.z);

  Serial.println("\tuTesla ");

  */



  delay(2000);

  Serial.println();

}



//

//

//

//

void cardinalDir(float headingDegrees)

{

  String cardinal;

  if (headingDegrees > 348.75 || headingDegrees < 11.25)

  {

    cardinal = " N";

  }

  else if (headingDegrees > 11.25 && headingDegrees < 33.75)

  {

    cardinal = " NNE";

  }

  else if (headingDegrees > 33.75 && headingDegrees < 56.25)

  {

    cardinal = " NE";

  }

  else if (headingDegrees > 56.25 && headingDegrees < 78.75)

  {

    cardinal = " ENE";

  }

  else if (headingDegrees > 78.75 && headingDegrees < 101.25)

  {

    cardinal = " E";

  }

  else if (headingDegrees > 101.25 && headingDegrees < 123.75)

  {

    cardinal = " ESE";

  }

  else if (headingDegrees > 123.75 && headingDegrees < 146.25)

  {

    cardinal = " SE";

  }

  else if (headingDegrees > 146.25 && headingDegrees < 168.75)

  {

    cardinal = " SSE";

  }

  else if (headingDegrees > 168.75 && headingDegrees < 191.25)

  {

    cardinal = " S";

  }

  else if (headingDegrees > 191.25 && headingDegrees < 213.75)

  {

    cardinal = " SSW";

  }

  else if (headingDegrees > 213.75 && headingDegrees < 236.25)

  {

    cardinal = " SW";

  }

  else if (headingDegrees > 236.25 && headingDegrees < 258.75)

  {

    cardinal = " WSW";

  }

  else if (headingDegrees > 258.75 && headingDegrees < 281.25)

  {

    cardinal = " W";

  }

  else if (headingDegrees > 281.25 && headingDegrees < 303.75)

  {

    cardinal = " WNW";

  }

  else if (headingDegrees > 303.75 && headingDegrees < 326.25)

  {

    cardinal = " NW";

  }

  else if (headingDegrees > 326.25 && headingDegrees < 348.75)

  {

    cardinal = " NNW";

  }

  Serial.println(cardinal);

}


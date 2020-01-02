/* BoatCodeStarterWind+GPS+Compass rev 7/31/2017
© 2014-2017 RoboSail
Full description under Description tab */

boolean displayValues = true;  //true calls function for values to be printed to monitor

#include <Servo.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <UsefulCalcs.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include "RoboSail.h"
//Fill in min/max parameters for the RC Receiver and WindSensor in RoboSail.h tab

#include <math.h>

Adafruit_GPS GPS(&Serial1);
// initialize utility that will convert lat/lon to (x,y) positions in meters
UsefulCalcs calc(false);


int count = 0;
bool isAuto = true;
int change = 3;
int angle = 90;
double P = 0.5; //Proportional Gain (Multiplication term used with present error)
double D = 0.2; //Differential Gain (Multiplication term used with future error)
double desiredCompassDirection; //Desired direction of boat travel.
double compassError; // Present error in direction of travel.
double distance; // Present distance from next goal in meters.
byte i = 0; // Index for course objective (Which desired location you are currently aiming for)
int desiredX[] = {-35, -35, -70, -50, 0}; // X coordinates in meters, stored in an array, of the route/course.
int desiredY[] = {-35, -135, -100, -50, 0}; // X coordinates in meters, stored in an array, of the route/course.
int rudderNeutral = 90; // Variable that you set to your personal boats neutral rudder position val
void setup() {
  rudderServo.write(angle);
  Serial.begin(115200);
  accel.begin();
  mag.begin();
  Serial.println("\nRoboSail BoatCode Wind+GPS+Compass-7/31/16");  //write program name/rev here
  // Set RC receiver and all Sensors on digital input pins
  Serial.println("hi");
  declarePins();
  
  checkGPS();

  checkCompass();
  delay(10000);
 }

void loop() {
//*********** Read in data from the RC receiver and sensors *********
  readReceiver();
  readAccel();
  readGPS();  //puts values in "start" and "relative" variable

  //Read heading and tilt from the Compass
  readCompassAccel();
  
// You now have values from the RC Receiver and Sensors in these variables: 
// rudderPosition, sailPosition, and windAngle, 
// pitchAccel, rollAccel, yawAccel, robosailRollAccel, heading,
// robosailHeading, robosailRoll,3
// startPositionX, startPositionY, relPositionX, relPositionY, angleFromStart
// as defined in the RoboSail.h tab
// Calculate new values for rudderPosition and sailPosition in degrees 
// and set those variables to the new values.
// If you do not set the values, it will use the values from the RC Receiver

//**************** your code here - add lines a-s needed ******************
  if (rudderPosition <= -90){
    i = 0;
    isAuto = !isAuto;
    delay(500);
  }

  if (i == sizeof(desiredX)) {
      isAuto = false;
       i = 0;
  }
    
  if (isAuto){
    
    automatic();
    rudderServo.write(rudderPosition);
  }
  else {
    driveRudderServo(rudderPosition);
  }

//  delay(100);
  Serial.print("RUDDER POSITION: "); Serial.print(rudderPosition);
  
/********************* send commands to motors *************************/
//  
   
  
if (displayValues) {printToMonitor();} 
} //end of loop()

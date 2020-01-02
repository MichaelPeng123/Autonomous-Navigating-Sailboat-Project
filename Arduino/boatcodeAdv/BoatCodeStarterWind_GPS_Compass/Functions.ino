/*********Functions for Setup ************/
void declarePins()
{
  pinMode(RUDDER_RC_PIN, INPUT);

  // attach the servos to the proper pins
  rudderServo.attach(RUDDER_SERVO_PIN);
}

void checkGPS()
{ //Set Up GPS and wait for fix on position
  GPS.begin(9600);  //default baud rate for Adafruit MTK GPS's
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);  //setting for minimum recommended data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  //update rate is 1 Hz
  enableInterrupt(); // activate TIMER0 interrupt, goes off every 1 msec
  while (start_pos_found == false)  //loop code will not start until GPS is ready
  {
    readGPS();
  }
}

void checkCompass()
{ // Set up Compass and check that it is connected
  mag.enableAutoRange(true);
  if (!mag.begin() || !accel.begin()) //Initialize the sensor
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("No LSM303 Compass detected ... Check your wiring!");
    while (1);
  }
}
/*********Functions to read RC Transmitter/Receiver and Sensors *****/
// Takes in the PWM signals from the RC Receiver and translate
// them to the servo positions in degrees.
// Takes in the PWM signals from the WindSensor and translate q

void readReceiver() {
  rudderPulseWidth = pulseIn(RUDDER_RC_PIN, HIGH);
//  Serial.print("RUDDER PULSE:" ); Serial.print(rudderPulseWidth);
//  Serial.println();
  rudderPosition = map(rudderPulseWidth, RUDDER_LOW, RUDDER_HIGH, -60, 60);
//  Serial.print("RUDDER POSITION:"); Serial.print(rudderPosition);
//  Serial.println();
}


void readAccel()   /* Read the Accelerometer event and put data in variables */
{
  accel.getEvent(&event);
  pitchAccel = event.acceleration.x;
  rollAccel = event.acceleration.y;
  yawAccel = event.acceleration.z;
  //define roll for RoboSail as rolling to Port side is positive, rolling to Starboard is negative
  robosailRollAccel  = -1 * rollAccel;
}
/************Functions to drive Sail and Rudder servos ****************/
// This code takes in the desired postions for the servos in degrees (as
// defined in RoboSail) then calculates appropriate values for the servo commands,
// making sure not to send the servos to impossible positions, which could
// damage the servo motors.
// The Rudder servo motor ranges from 0 to 180 with 90 deg in the center
// The Sailwinch servo is at ~55 deg when full-in, which we think of as 0 deg,
// and ~125 deg when full out, which we think of as 90 deg

void driveRudderServo(int rudderPos)
{
  //  if ((rudderPos >= -60) && (rudderPos <= 60))
  //  {
  rudderServoOut = map(rudderPos, -90, 90, 0, 180);
  rudderServo.write(rudderServoOut);
  //  }
  //  else
  //  {
  //    Serial.print("ERROR - rudder position out of range: ");
  //    Serial.println(rudderPos);
  //  }
  //  Serial.println(rudderPos);
}
/****************************************************/

// Function to Print out all values for debug.
void printToMonitor()
{

  //  Serial.print("  Rudder, from RC: ");
  //  Serial.print(rudderPulseWidth);
  Serial.print("\n"); // Print a new line
  //
  Serial.print("Fix: "); Serial.print(GPSfix);
  Serial.print(" quality: "); Serial.print(GPSqual);
  Serial.print(" satellites: "); Serial.println(GPSsat);
  Serial.print("x = "); Serial.print(relPositionX);
  Serial.print("   y = "); Serial.print(relPositionY);
  Serial.print("  angle from start = "); Serial.println(angleFromStart);

  //  Serial.print("Roll raw value: "); Serial.print(robosailRollAccel);
  //  Serial.print("Roll in deg: "); Serial.print(robosailRoll);
  //  Serial.print(",              Heading: "); Serial.println(robosailHeading);
  //  Serial.println();
}

//void manuel(){
//  rudderPosition = map(rudderPosition, -90, 90, 0, 180);
//  rudderServo.write(rudderPosition);
//}

void automatic() {
  distance = sqrt(pow(desiredX[i] - relPositionX, 2) + pow(desiredY[i] - relPositionY, 2));
  //desiredCompassDirection = arctan((desiredY[i] - relPositionY)/(desiredX[i] - relPositionX));
  desiredCompassDirection = atan2 ((desiredY[i] - relPositionY), (desiredX[i] - relPositionX)); //East would
  //be zero here which is why they probably use east as zero for robosailHeading
  desiredCompassDirection = desiredCompassDirection * 57296 / 1000; // Turns it from radians into degrees.
  compassError = desiredCompassDirection - robosailHeading; // Might not be quite right think through logic together as a class.

  Serial.print("DESIREDCOMPASS ERROR: "); Serial.print(desiredCompassDirection);
  Serial.print("COMPASS ERROR: "); Serial.print(compassError);

  // Corrects the range of error to be plus 180 to minus 180.
  if (compassError <= -180) {
    compassError += 360;
  }
  else if (compassError >= 180) {
    compassError -= 360;
  }
  compassError = -1 * compassError;
  // Control Function
  rudderPosition = rudderNeutral + P * compassError;



  rudderPosition = 180-rudderPosition;
  ////still need to fix this
  if(rudderPosition <= 0){
  rudderPosition = 0;
  }
  else if(rudderPosition >= 180){
  rudderPosition= 180;
  }


  // you are at goal, move to next goal (still need to define end goal state)
  if (distance <= 3) {
    i = i + 1;
  }
}

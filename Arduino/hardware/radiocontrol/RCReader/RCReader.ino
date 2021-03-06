/* RCReader rev rev 7/30/2017
© 2014-2017 RoboSail
This program puts the Arduino micro-computer in the RC (Radio Control) system 
It takes in the control signals coming in from the Receiver and 
displays the following to the Serial Monitor:
  - The actual "pulse" coming in from the receiver for each channel
    (typical range of 1000 - 2000)
  - the angle at which the rudder or sail should be positioned 
    given that command (in the RoboSail frame of reference) 

This program helps the user determine 
  - if they are reading good signals from the receiver (range of 1000 - 2000)
  - if the Arduino computer is functioning correctly

Rudder and Sail data from the RC receiver are read in on 
digital pins 2 and 3 respectively.
*/

#include <Servo.h>

// Pin assignments
//input pins from receiver
#define Throttle 3
// variables to hold input values
int throttlePulse;


void setup() {
  Serial.begin(115200);
  Serial.println("\nRCReader code - RoboSail");
  // Set RC receiver on digital input pins
  pinMode(Throttle, INPUT);
}

void loop() {
  // Read commanded (manual) values from the RC reciever
  // pulseIn returns the width of the command pulse in microseconds.
  throttlePulse = pulseIn(Throttle, HIGH);

  // Print out the values for debug.
  Serial.print("Throttle pulse from receiver: ");
  Serial.print(throttlePulse);
 
}

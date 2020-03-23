/****************************************************************
  Adapted from
  ProximityTest.ino
  APDS-9960 RGB and Gesture Sensor
  SparkFun Electronics
****************************************************************/

#include <Wire.h>
#include "SparkFun_APDS9960.h"

#define ECHO 12
#define TRIG 7

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint8_t proximity_data = 0;
unsigned int distance_mm = 0; //This variable will hold the distance

void setup() {
  // Initialize Serial port
  Serial.begin(9600);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    // Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Adjust the Proximity sensor gain
  if ( !apds.setProximityGain(PGAIN_2X) ) {
    Serial.println(F("Something went wrong trying to set PGAIN"));
  }

  // Start running the APDS-9960 proximity sensor (no interrupts)
  if ( apds.enableProximitySensor(false) ) {
    // Serial.println(F("Proximity sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during sensor init!"));
  }
  // Initialise the Ultrasonic sensor
  pinMode(ECHO, INPUT); //Initialise pin 12 as an input
  pinMode(TRIG, OUTPUT); //Initialise pin 7 as an output
}



void loop() {
  Serial.println("time,APDS9960,HCSR04_mm");
  for (int i = 0; i < 100; ++i)
  {
    // Read the proximity value
    if ( !apds.readProximity(proximity_data) ) {
      Serial.println("Error reading proximity value");
      break;// breaks out of the for loop early
    }
    //Read ultrasonic data
    //TODO: call the function sonar_mm and store the result in distance_mm

    //Print the CSV data
    Serial.print(i);
    Serial.print(",");
    Serial.print(proximity_data);
    Serial.print(",");
    Serial.println(distance_mm);

    // Wait 200 ms before next reading
    delay(200);
  }
  while (1) {}; //infinite loop to stop the experiment
}

unsigned int sonar_mm(void) {
  long duration = 0;
  const float speed_sound = 340.29;// m/s, "const" makes the compiler able to optimise the program where this variable is used, cool!
  // Read in a distance from the ultrasonic distance sensor:
  // The ultrasonic burst is triggered by a HIGH pulse of 10 microseconds.
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  //read length of time pulse
  duration = pulseIn(ECHO, HIGH); //This function measures a pulsewidth and returns the width in microseconds
  // convert the time into a distance
  // the code "(unsigned int)" turns the result of the distance calculation
  // into an integer instead of a floating point (decimal or fractional) number.
  return (unsigned int)(0.5 * duration * 1e-6 * speed_sound * 1e3);
  //"unsigned" ensures we are returning an unsigned number, remember that there is no such thing as negative distance.
}

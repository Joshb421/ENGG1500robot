#include "pins.h"
#include "robot.h"
#include <Arduino.h>

const int minVals[4]={28,25,26,25};
const int maxVals[4]={1020,976,947,972};

void PIDLine(int motorDefault, float Kp, float Ki, float Kd);

void setup() {
  robotInit();
}

void loop() {
  PIDLine(120, 10, 0,   1);
  delay(15);
}

void PIDLine(int motorDefault, float Kp, float Ki, float Kd) {
  static float lastError = 0, accum = 0;
  float error = lineReading();
  float errorRate = error - lastError;
  if (abs(error + accum) < 20) {
    accum += error;
  }
  lastError = error;
  float correction = Kp * error + Ki * accum + Kd * errorRate;
  Serial.print("Error");
  Serial.println(error);
  if (abs(correction) < 60) {
    moveLineOL(1,1, (byte)motorDefault +correction, (byte)motorDefault - correction, 20);
  }
  else if (correction > 60) {
    moveLineOL(0,1, 170, 170, 20); //Runs one wheel in reverse if the error is too great
  }
  else if (correction < -60) {
    moveLineOL(1,0, 170, 170, 20); //Runs one wheel in reverse if the error is too great
  }
}

#include "pins.h"
#include "robot.h"
#include <Arduino.h>

void followLine();

float lineOffset = 3.0;
byte lineDuty = 130,  turnDuty = 120;
float turningFactor = 5;
int sensorVals[4]={0,0,0,0};
bool sensorBoolVals[4]={0,0,0,0};

void PIDLine(int motorDefault, float Kp, float Ki, float Kd);

void setup(){
  robotInit();
}

void loop(){
  PIDLine(75,10,1,8);
}

void PIDLine(int motorDefault, float Kp, float Ki, float Kd){
  static float lastError = 0, accum = 0;
  float error = lineReading();
  float errorRate = error - lastError;
  if (abs(error + accum) < 20) {
      accum += error;
  }

  lastError = error;

  Serial.println(error);
  Serial.println(errorRate);
  Serial.println(accum);
  Serial.println();
  float correction = Kp * error + Ki * accum + Kd * errorRate;
  Serial.println(correction);
  Serial.println();
  
  moveLineOL(1,(byte)motorDefault+correction,(byte)motorDefault-correction,10);

}

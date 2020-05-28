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
  moveLineOL(1, 100, 100, 50);
}

void loop(){
  PIDLine(105,15,0,0);
  delay(50);
}

void PIDLine(int motorDefault, float Kp, float Ki, float Kd){
  static float lastError = 0, accum = 0;
  float error = lineReading(400);
  float errorRate = error - lastError;
  if (abs(error + accum) < 20) {
      accum += error;
  }
  lastError = error;
  float correction = Kp * error + Ki * accum + Kd * errorRate;
  Serial.print("Error");
  Serial.println(error);
  Serial.print("Correction: ");
  Serial.println(correction);
  moveLineOL(1,(byte)motorDefault+correction,(byte)motorDefault-correction,25);

}

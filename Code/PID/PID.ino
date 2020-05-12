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
  PIDLine(150,1,.1,0);
}

int mapDuty(float speed){
  speed = map(speed,0,100,50,84);
  int duty = -0.0017*pow(speed,3)+0.41*pow(speed,2)-28.266*speed+702;
  if (!duty){
    Serial.println("Warning: duty too low to make the motor move");
  }
  return duty;
}

void PIDLine(int motorDefault, float Kp, float Ki, float Kd){
  float error=0;
  float prop=0;
  float deriv=0;
  float integ=0;
  float prev=0;
  float accumErr=0;
  float correction=0;
  int integCap = 20;
  error = lineReading(400);
  Serial.print("Error: ");
  Serial.println(error);
  if (accumErr<integCap){
          accumErr += error;
  }
  prop = error*Kp;
  integ = accumErr*Ki;
  deriv = (error-prev)*Kd;
  correction = prop+integ+deriv;
  Serial.print("Correction: ");
  Serial.println(correction); 
  prev=error;
  moveLineOL(1,mapDuty(motorDefault+correction),mapDuty(motorDefault-correction),20);
}

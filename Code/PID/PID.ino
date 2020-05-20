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
  PIDLine(75,1,1,0);
  Serial.println(analogRead(A3));
  Serial.println(analogRead(A2));
  Serial.println(analogRead(A1));
  Serial.println(analogRead(A0));
}

void PIDLine(int motorDefault, float Kp, float Ki, float Kd){
  float error=0;
  float prop=0;
  float deriv=0;
  float integ=0;
  static float prev=0;
  static float accumErr=0;
  float correction=0;
  int integCap = 20;
  error = lineReading(400);
  //Serial.print("Error: ");
  //Serial.println(error);
  accumErr += error;
  if (accumErr>integCap){
      accumErr=integCap;
  }
  else if (accumErr<-integCap){
      accumErr=-integCap;
  }
  prop = error*Kp;
  integ = accumErr*Ki;
  deriv = (error-prev)*Kd;
  correction = prop+integ+deriv;
  //Serial.print("Correction: ");
  //Serial.println(correction); 
  prev=error;
  moveLineOL(1,(byte)motorDefault+correction,(byte)motorDefault-correction,20);

}

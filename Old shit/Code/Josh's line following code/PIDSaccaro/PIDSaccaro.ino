#include "pins.h"
#include "robot.h"
#include <Arduino.h>


void followLine();

float lineOffset = 3.0;
byte lineDuty = 130,  turnDuty = 120;
float turningFactor = 5;
int sensorVals[4] = {0, 0, 0, 0};
bool sensorBoolVals[4] = {0, 0, 0, 0};
int lostCounter = 0;


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
  float error = lineReading(400);

  float errorRate = error - lastError;
  if (abs(error + accum) < 20) {
    accum += error;
  }
  lastError = error;
  float correction = Kp * error + Ki * accum + Kd * errorRate;
  Serial.print("Error");
  Serial.println(error);
  if(digitalRead(A4)&&digitalRead(A5)){
    //Serial.println("fork detected");
    correction -=10 ;
  }
  if (abs(correction) < 60) {
    moveLineOL(1, (byte)motorDefault +correction, (byte)motorDefault - correction, 20);
    //Serial.println("standard routine");
  }
  else if (correction > 60) {
    analogWrite(ENA, 170); //Set Motor speeds
    analogWrite(ENB, 170);
    digitalWrite(MOT1, 0); //Set polarity for the motors
    digitalWrite(MOT2, 1); //Dir is inverted (!) to create a voltage differnetial
    digitalWrite(MOT3, 1);
    digitalWrite(MOT4, 0);
    delay(20);
    analogWrite(ENA, 0); //Set Motor speeds
    analogWrite(ENB, 0);
    //Serial.println("sharp routine");
  }
  else if (correction < -60) {
    analogWrite(ENA, 170); //Set Motor speeds
    analogWrite(ENB, 170);
    digitalWrite(MOT1, 1); //Set polarity for the motors
    digitalWrite(MOT2, 0); //Dir is inverted (!) to create a voltage differnetial
    digitalWrite(MOT3, 0);
    digitalWrite(MOT4, 1);
    delay(20);
    analogWrite(ENA, 0); //Set Motor speeds
    analogWrite(ENB, 0);
    //Serial.println("sharp routine");
  }
//  else if (correction > 20 && correction < 70) {
//    moveLineOL(1, (byte)motorDefault + abs(correction), 0, 15);
//    Serial.println("moderate routine");
//  }
//  else if (correction < -20 && correction > -70) {
//    moveLineOL(1, 0, (byte)motorDefault + abs(correction), 15);
//    Serial.println("moderate routine");
//
//  }
//  else if (correction > 70) {
//    analogWrite(ENA, 150); //Set Motor speeds
//    analogWrite(ENB, 150);
//    digitalWrite(MOT1, 0); //Set polarity for the motors
//    digitalWrite(MOT2, 1); //Dir is inverted (!) to create a voltage differnetial
//    digitalWrite(MOT3, 1);
//    digitalWrite(MOT4, 0);
//    delay(15);
//    analogWrite(ENA, 0); //Set Motor speeds
//    analogWrite(ENB, 0);
//    Serial.println("sharp routine");
//  }
//  else if (correction < -70) {
//    analogWrite(ENA, 150); //Set Motor speeds
//    analogWrite(ENB, 150);
//    digitalWrite(MOT1, 1); //Set polarity for the motors
//    digitalWrite(MOT2, 0); //Dir is inverted (!) to create a voltage differnetial
//    digitalWrite(MOT3, 0);
//    digitalWrite(MOT4, 1);
//    delay(15);
//    analogWrite(ENA, 0); //Set Motor speeds
//    analogWrite(ENB, 0);
//    Serial.println("sharp routine");
//
//  }
//  else {
//  }


}

#ifndef ROBOT
#define ROBOT

#include <Arduino.h>
#include "pins.h"

//Init functions
void robotInit();
void motorInit();
void sonarInit();
void lineInit();


//Motor control functions
void LeftISR();
void RightISR();
bool moveDone(bool left, bool right);

//Motor interface functions
void moveLineCL(bool dir, byte duty, int distanceInmm);
void turnCL(bool dir, byte duty, int distanceInmm);
void moveLineOL(bool dir, byte dutyL, byte dutyR, int pause);
void turnOL(bool dir, byte duty, int pause);

//Sensor functions
float measureDistance(int samples);
float lineReading(int thresholdVal);


#endif

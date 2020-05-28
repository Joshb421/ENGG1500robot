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
void turnCL(bool dir, bool duty, int distanceInmm);
void moveLineOL(bool dir, byte duty, int time);
void turnOL(bool dir, bool duty, int time);

//Sensor functions
float measureDistance(int samples);
float lineReading();


#endif

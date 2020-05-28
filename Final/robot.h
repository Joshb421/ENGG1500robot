#ifndef ROBOT
#define ROBOT

#include <Arduino.h>
#include "pins.h"

//Init functions
void robotInit();
void motorInit();
void sonarInit();
void lineInit();
void followWall(float kp);

//Motor control functions

//Motor interface functions

void moveLineOL(bool dirL, bool dirR, byte dutyL, byte dutyR, int pause);

//Sensor functions
float measureDistance(int samples, byte angle, int timeOut);
float lineReading();

void lineLost();


#endif

#ifndef ROBOT
#define ROBOT

#include <Arduino.h>

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
void moveLine(bool dir, byte duty, int distanceInmm);

//Sensor functions
unsigned float measureDistance(int samples);
float lineReading();


#endif

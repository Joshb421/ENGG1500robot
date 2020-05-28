#include <pins.h>
#include <robot.h>
#include <Arduino.h>


void followLine(byte duty);

float lineOffset = 3.0;
byte lineDuty = 130,  turnDuty = 120;
float turningFactor = 5;

void setup(){
  robotInit();
  motorInit();
  lineInit();
}

void loop(){
  followLine();
}

void followLine(){
    float distance = lineReading()-lineOffset;
    Serial.print("Distance from line: ");
    Serial.println(distance);
    if (distance<1&&distance>-1){
      moveLineOL(1,lineDuty,50);
    }
    else if (distance>1&&distance<50){
      turnOL(1,turnDuty,distance*turningFactor);
    }
    else if (distance<-1&&distance>-50){
      turnOL(0,turnDuty,distance*turningFactor);
    }

float d = measureDistance(10);
    if (d => RangeMin && d =< RangeMax){
      currentState = TURN; //Turns when withon the specified range value
    }
  
    if (digitalRead(IRDigitalOut)) {
      currentState = WALKFRONT; //Will go when senses black
    }    
}

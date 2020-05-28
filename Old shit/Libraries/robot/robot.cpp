#include "robot.h"
#include "pins.h"
//This function simply calls other functions needed for the init process
void robotInit(){
   Serial.begin(9600);
   motorInit();//Motors
   sonarInit();//Sonar sensor
}

//Configures appropriate pins and paramaters for the motors
void motorInit(){
  const float pulsesPerMM = 0.09794150344; //Calclated using encoderRes/pi*diamater
  bool dir = 1; //Sets Default direction to forward
  volatile byte rightStepsToGo; //Variables used for semi-closed loop control of motors
  volatile byte leftStepsToGo;  //Volatitle indicates that they are used by the ISRs
  volatile byte rightCounter;
  volatile byte leftCounter;
  volatile bool rightFlag;
  volatile bool leftFlag;
  pinMode(ENA,OUTPUT); //set ENA as an output
  pinMode(ENA,OUTPUT); //set ENB as an output
  pinMode(MOT1,OUTPUT); //set IN1 as an output
  pinMode(MOT2,OUTPUT); //set IN2 as an output
  pinMode(MOT3,OUTPUT); //set IN3 as an output
  pinMode(MOT4,OUTPUT); //set IN4 as an output
  attachInterrupt(digitalPinToInterrupt(ENCL), LeftISR, FALLING);//Left encoder
  attachInterrupt(digitalPinToInterrupt(ENCR), RightISR, FALLING);//Right encoder
  Serial.println("Motors are ready");
}
//Sets up ultrasonic sensor
void sonarInit(){
  pinMode(TRIG, OUTPUT);
  const float speed_sound = 340.29;
  Serial.println("Ultrasonic sensor is ready");
}

void lineInit(){
  const int sensor[4] = {A3, A2, A1, A0};
  const float sensorX[4] = {22.5f, 7.5f, -7.5f, -20.0f};
   for (int i = 0; i < 4; i++) {
    pinMode(sensor[i]),INPUT);
  }
  Serial.println("Line sensors ready");
}

//ISRs for the motor encoders, designed to be as light as possible
void LeftISR() {
  leftCounter++;
  if (leftStepsToGo == leftCounter) {
    leftFlag = 1;
  }
}
void RightISR() {
  rightCounter++;
  if (rightStepsToGo == rightCounter) {
        rightFlag = 1;
  }
}
//This is used to monitor flags to see if the motors are done their movment, and if so, turn them off
bool moveDone(bool left, bool right) {
  leftCounter = !left*leftCounter; //Resets counter if move done is called
  leftCounter = !right*leftCounter; //Resets counter if move done is called
  if(left){
    analogWrite(5,0);
  }
  if (right){
    analogWrite(5,0);
  }
  return left*right; //Only returns one if both flags are one - Can be used to wait for something to be done
}

//Function to move in a straight-ish line, through a semi closed loop methodology
//Forward if dir = 1
//
void moveLine(bool dir, byte duty, int distanceInmm) {
  rightCounter = 0;
  leftCounter = 0;
  rightCounter = distanceInmm * pulsesPerMM;
  leftCounter = distanceInmm * pulsesPerMM;
  error += distanceInmm - rightCounter / pulsesPerMM;
  analogWrite(ENA, duty); //Set Motor speeds
  analogWrite(ENB, duty);
  digitalWrite(MOT1, dir); //Set polarity for the motors
  digitalWrite(MOT2, !dir); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(MOT3, dir);
  digitalWrite(MOT4, !dir);
  while (!movedone) {
  }
}

//Function which uses the Sonar sensor to measure a distance in mm
//TODO: Implement the ability to scan at different angles
//TODO: Implement a timeout system 
float measureDistance(int samples, byte angle, int timeOut) {
  long duration = 0;
  float sum;
  sum = 0;
  for(int n = 0; n<samples;n++){ //Takes a number of samples to reduce hysterisis
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    duration = pulseIn(ECHO, HIGH); 
    sum += (0.5 * duration * 1e-6 * speed_sound * 1e3); //Creates a running average 
  }
  return sum/samples; //Returns the average of some number of samples
}

//Function to use the 4 front sensors for line detection
//
float lineReading() {
  float num = 0;
  float den = 0;
  for (int i = 0; i < 4; i++) {
    int value = analogRead(sensor[i]);
    num += value * sensorX[i];
    den += value;
  }
  return (num / den);
}

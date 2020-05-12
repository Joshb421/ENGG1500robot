#include "robot.h"

volatile byte rightStepsToGo; //Variables used for semi-closed loop control of motors
volatile byte leftStepsToGo;  //Volatitle indicates that they are used by the ISRs
volatile byte rightCounter;
volatile byte leftCounter;
volatile bool rightFlag;
volatile bool leftFlag;
const float pulsesPerMM = 0.09794150344; //Calclated using encoderRes/pi*diamater
const int sensor[4] = {A3, A2, A1, A0};
const float sensorX[4] = {22.5f, 7.5f, -7.5f, -22.5f};

//This function simply calls other functions needed for the init process
void robotInit(){
   Serial.begin(57600);
   motorInit();//Motors
   sonarInit();//Sonar sensor
   lineInit();
}

//Configures appropriate pins and paramaters for the motors
void motorInit(){
  bool dir = 1; //Sets Default direction to forward
  pinMode(ENA,OUTPUT); //set ENA as an output
  pinMode(ENB,OUTPUT); //set ENB as an output
  pinMode(MOT1,OUTPUT); //set IN1 as an output
  pinMode(MOT2,OUTPUT); //set IN2 as an output
  pinMode(MOT3,OUTPUT); //set IN3 as an output
  pinMode(MOT4,OUTPUT); //set IN4 as an output
  //attachInterrupt(digitalPinToInterrupt(2), LeftISR, FALLING);//Left encoder
  //attachInterrupt(digitalPinToInterrupt(3), RightISR, FALLING);//Right encoder
  Serial.println("Motors are ready");
}
//Sets up ultrasonic sensor
void sonarInit(){
  pinMode(TRIG , OUTPUT);
  Serial.println("Ultrasonic sensor is ready");
}

void lineInit(){
   for (int i = 0; i < 4; i++) {
    pinMode(sensor[i],INPUT);
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
    analogWrite(ENA,0);
  }
  if (right){
    analogWrite(ENB,0);
  }
  return left*right; //Only returns one if both flags are one - Can be used to wait for something to be done
}

//Function to move in a straight-ish line, through a semi closed loop methodology
//Forward if dir = 1
//
void moveLineCL(bool dir, byte duty, int distanceInmm) {
  rightCounter = 0;
  leftCounter = 0;
  rightCounter = distanceInmm * pulsesPerMM;
  leftCounter = distanceInmm * pulsesPerMM;
  analogWrite(ENA, duty); //Set Motor speeds
  analogWrite(ENB, duty);
  digitalWrite(MOT1, dir); //Set polarity for the motors
  digitalWrite(MOT2, !dir); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(MOT3, dir);
  digitalWrite(MOT4, !dir);
  while (!moveDone) {
  }
}

void moveLineOL(bool dir, byte dutyLeft, byte dutyRight, int pause) {
  analogWrite(ENA, dutyLeft); //Set Motor speeds
  analogWrite(ENB, dutyRight);
  digitalWrite(MOT1, dir); //Set polarity for the motors
  digitalWrite(MOT2, !dir); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(MOT3, dir);
  digitalWrite(MOT4, !dir);
  delay(pause);
  analogWrite(ENA, 0); //Set Motor speeds
  analogWrite(ENB, 0);
}

 //Function to move the robot in a turn (with only one wheel moving)
void turnCL(bool dir, byte duty, int distanceInmm){
  digitalWrite(MOT1, 1); //Set polarity for the motors
  digitalWrite(MOT2, 0); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(MOT3, 1);
  digitalWrite(MOT4, 0);
  rightCounter = 0;
  leftCounter = 0;
  if (dir == 1){
    rightCounter = distanceInmm * pulsesPerMM;
    analogWrite(ENA, duty);
  }
  else{
    rightCounter = distanceInmm * pulsesPerMM;
    analogWrite(ENB, duty);
  }
  while (!moveDone) {
  }
}


//Function which uses the Sonar sensor to measure a distance in mm
//TODO: Implement the ability to scan at different angles
//TODO: Implement a timeout system 
float measureDistance(int samples, byte angle, int timeOut) {
  const float speed_sound = 340.29;
  long duration = 0;
  float sum;
  sum = 0;
  for(int n = 0; n<samples;n++){ //Takes a number of samples to reduce hysterisis
    digitalWrite(TRIG , HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG , LOW);
    duration = pulseIn(ECHO , HIGH); 
    sum += (0.5 * duration * 1e-6 * speed_sound * 1e3); //Creates a running average 
  }
  return sum/samples; //Returns the average of some number of samples
}

//Function to use the 4 front sensors for line detection
//
float lineReading(int thresholdVal) {
  float num = 0;
  float den = 0;
  for (int i = 0; i < 4; i++) {
    int value = analogRead(sensor[i]);
    num += value * sensorX[i];
    den += value;
  }
  return (num / den);
}


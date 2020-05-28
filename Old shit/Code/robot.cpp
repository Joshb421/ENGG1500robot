#include "robot.h"
#include <servo.h>

volatile byte rightStepsToGo; //Variables used for semi-closed loop control of motors
volatile byte leftStepsToGo;  //Volatitle indicates that they are used by the ISRs
volatile byte rightCounter;
volatile byte leftCounter;
volatile bool rightFlag;
volatile bool leftFlag;
const float pulsesPerMM = 0.09794150344; //Calclated using encoderRes/pi*diamater
const int sensor[4] = {A3, A2, A1, A0};
const float sensorX[4] = {22.5f, 7.5f, -7.5f, -20.0f};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//This function simply calls other functions needed for the init process
void robotInit(){
   Serial.begin(9600);
   motorInit();//Motors
   sonarInit();//Sonar sensor
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Configures appropriate pins and paramaters for the motors
void motorInit(){
  bool dir = 1; //Sets Default direction to forward
  pinMode(ENA,OUTPUT); //set ENA as an output
  pinMode(ENB,OUTPUT); //set ENB as an output
  pinMode(MOT1,OUTPUT); //set IN1 as an output
  pinMode(MOT2,OUTPUT); //set IN2 as an output
  pinMode(MOT3,OUTPUT); //set IN3 as an output
  pinMode(MOT4,OUTPUT); //set IN4 as an output
  attachInterrupt(digitalPinToInterrupt(2), LeftISR, FALLING);//Left encoder
  attachInterrupt(digitalPinToInterrupt(3), RightISR, FALLING);//Right encoder
  Serial.println("Motors are ready");
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void lineInit(){
   for (int i = 0; i < 4; i++) {
    pinMode(sensor[i],INPUT);
  }
  Serial.println("Line sensors ready");
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void moveLineOL(bool dir, byte duty, int time) {
  analogWrite(ENA, duty); //Set Motor speeds
  analogWrite(ENB, duty);
  digitalWrite(MOT1, dir); //Set polarity for the motors
  digitalWrite(MOT2, !dir); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(MOT3, dir);
  digitalWrite(MOT4, !dir);
  delay(time);
  analogWrite(ENA, 0); //Set Motor speeds
  analogWrite(ENB, 0);
}

//Function to move the robot in a turn (with only one wheel moving)-----------------------------------------------------------------------------------------------------------------------------------------
void turnCL(bool dir, bool duty, int distanceInmm){
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
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void turnOL(bool dir, bool duty, int time){
  digitalWrite(MOT1, 1); //Set polarity for the motors
  digitalWrite(MOT2, 0); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(MOT3, 1);
  digitalWrite(MOT4, 0);
  if (dir == 1){
    analogWrite(ENA, duty);
  }
  else{
    analogWrite(ENB, duty);
  }
  delay(time);
}

//Function which uses the Sonar sensor to measure a distance in mm------------------------------------------------------------------------------------------------------------------------------------------

const int trigPin = 12;
const int echoPin = 13;
long duration;
int distance;
int rotate;

Servo myServo; // Creates a servo object for controlling the servo motor

void sonarInit() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  // TO DO CHECK WHICH PIN SERVO IS ATTACHED TO
  myServo.attach(4); // Defines on which pin is the servo motor attached
}
int scan() {
  // rotates from 90 degrees to 20 degrees but doesn't initiate scaning
  for(int i=90;i>=20;i--; rotate++){  
  myServo.write(i);
  delay(75);
  }
  
  // rotates the servo motor from 20 to 160 degrees
  for(int i=20;i<=160;i++;rotate++){  
  myServo.write(i);
  delay(75);
  distance = calculateDistance();// Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
  Serial.print(i); // Sends the current degree into the Serial Port
  Serial.print(","); // Sends addition character right next to the previous value
  Serial.print(distance); // Sends the distance value into the Serial Port
  Serial.print("."); // Sends addition character right next to the previous value
  }
  
  // Resets the previous lines from 160 to 90 degrees
  for(int i=160;i>=90;i--;rotate<=310;rotate++){  
  myServo.write(i);
  delay(75);
  distance = calculateDistance();
  Serial.print(i);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
  }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance(){ 
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance= duration*0.034/2;
  return distance;
}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

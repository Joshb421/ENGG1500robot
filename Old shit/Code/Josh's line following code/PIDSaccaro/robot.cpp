#include <Servo.h>
#include "robot.h"
Servo USServo;
bool lineHistory[4][20];
bool historyPtr = 0;
byte timesLost = 0;
volatile byte rightStepsToGo; //Variables used for semi-closed loop control of motors
volatile byte leftStepsToGo;  //Volatitle indicates that they are used by the ISRs
volatile byte rightCounter;
volatile byte leftCounter;
volatile bool rightFlag;
volatile bool leftFlag;
const float pulsesPerMM = 0.09794150344; //Calclated using encoderRes/pi*diamater
const int sensor[4] = {A3, A2, A1, A0};
const int sensorPins[3] = {A4, A5, 3};
const int sensorOffsets[4] = {0, 0, 0, 0};
const int sensorThresholds[4] = {6, 6, 6, 6};
const float sensorX[4] = {22.5, 7.5, -7.5, -22.5};
const int digitalSensors[5] = {A4, 7, 3, 2, A5};
const int minVals[4] = {24, 25, 23, 25};
const int maxVals[4] = {173, 364, 139, 414};
float Distances [3] = {0, 0, 0};



//This function simply calls other functions needed for the init process
void robotInit() {
  Serial.begin(57600);
  motorInit();//Motors
  sonarInit();//Sonar sensor
  lineInit();
}

//Configures appropriate pins and paramaters for the motors
void motorInit() {
  bool dir = 1; //Sets Default direction to forward
  pinMode(ENA, OUTPUT); //set ENA as an output
  pinMode(ENB, OUTPUT); //set ENB as an output
  pinMode(MOT1, OUTPUT); //set IN1 as an output
  pinMode(MOT2, OUTPUT); //set IN2 as an output
  pinMode(MOT3, OUTPUT); //set IN3 as an output
  pinMode(MOT4, OUTPUT); //set IN4 as an output
  //attachInterrupt(digitalPinToInterrupt(2), LeftISR, FALLING);//Left encoder
  //attachInterrupt(digitalPinToInterrupt(3), RightISR, FALLING);//Right encoder
  Serial.println("Motors are ready");
}
//Sets up ultrasonic sensor
void sonarInit() {
  pinMode(13, OUTPUT);
  USServo.attach(4);
  Serial.println("Ultrasonic sensor is ready");
}

void lineInit() {
  for (int i = 0; i < 4; i++) {
    pinMode(sensor[i], INPUT);
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
  leftCounter = !left * leftCounter; //Resets counter if move done is called
  leftCounter = !right * leftCounter; //Resets counter if move done is called
  if (left) {
    analogWrite(ENA, 0);
  }
  if (right) {
    analogWrite(ENB, 0);
  }
  return left * right; //Only returns one if both flags are one - Can be used to wait for something to be done
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

void moveLineOL(bool dir, byte dutyL, byte dutyR, int pause) {
  analogWrite(ENA, dutyL); //Set Motor speeds
  analogWrite(ENB, dutyR);
  digitalWrite(MOT1, dir); //Set polarity for the motors
  digitalWrite(MOT2, !dir); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(MOT3, dir);
  digitalWrite(MOT4, !dir);
  delay(pause);
  analogWrite(ENA, 0); //Set Motor speeds
  analogWrite(ENB, 0);
}

//Function to move the robot in a turn (with only one wheel moving)
void turnCL(bool dir, byte duty, int distanceInmm) {
  digitalWrite(MOT1, 1); //Set polarity for the motors
  digitalWrite(MOT2, 0); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(MOT3, 1);
  digitalWrite(MOT4, 0);
  rightCounter = 0;
  leftCounter = 0;
  if (dir == 1) {
    rightCounter = distanceInmm * pulsesPerMM;
    analogWrite(ENA, duty);
  }
  else {
    rightCounter = distanceInmm * pulsesPerMM;
    analogWrite(ENB, duty);
  }
  while (!moveDone) {
  }
}

void turnOL(bool dir, byte duty, int pause) {
  digitalWrite(MOT1, 1); //Set polarity for the motors
  digitalWrite(MOT2, 0); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(MOT3, 1);
  digitalWrite(MOT4, 0);
  if (dir == 1) {
    analogWrite(ENA, duty);
  }
  else {
    analogWrite(ENB, duty);
  }
  delay(pause);
}

//Function which uses the Sonar sensor to measure a distance in mm
//TODO: Implement the ability to scan at different angles
//TODO: Implement a timeout system
float measureDistance(int samples, byte angle, int timeOut) {
  USServo.write(angle);
  static byte lastAngle;
  if (lastAngle != angle) {
    delay(1000);
  }
  const float speed_sound = 340.29;
  long duration = 0;
  float sum;
  sum = 0;
  lastAngle = angle;
  for (int n = 0; n < samples; n++) { //Takes a number of samples to reduce hysterisis
    digitalWrite(13 , HIGH);
    delayMicroseconds(10);
    digitalWrite(13 , LOW);
    duration = pulseIn(12 , HIGH, timeOut * 1000);
    sum += (0.5 * duration * 1e-6 * speed_sound * 1e3); //Creates a running average
  }
  sum = sum / samples; //Returns the average of some number of samples
  return sum;
}

//Function to use the 4 front sensors for line detection
//
float lineReading(int thresholdVal) {
  float num = 0;
  float den = 0;
  float value = 0;
  byte sensorSum = 0;
  static byte lineLostCount = 0;
  for (int i = 0; i < 4; i++) {
    value = map(analogRead(sensor[i]), minVals[i], maxVals[i], 5, 100);
    //Serial.print(value);
    num += value * sensorX[i];
    den += value;
  }
  for (int i = 0; i < 5; i++) {
    sensorSum += digitalRead(digitalSensors[i]);
  }
  Serial.println(sensorSum);
  if (sensorSum < 1) {
    lineLostCount++;
    Serial.print("Line has been lost ");
    Serial.print(lineLostCount);
    Serial.println(" Times");
  }
  if (lineLostCount > 10) {
    Serial.println("Line has been lost too many times, changing strategies...");
    lineLostCount = 0;
    lineLost();
  }
  //  value = 3.1359*(num/den) - 8.956+2;
  return (num / den);
}

void lineLost() {
  Distances[0] = measureDistance(1, 15, 3);
  Serial.println(Distances[0]);
  Distances[1] = measureDistance(1, 90, 3);
  Serial.println(Distances[1]);
  Distances[2] = measureDistance(1, 165, 3);
  Serial.println(Distances[2]);
  USServo.write(90);
  if (Distances[1] < 100 && Distances[1] > 5) {
    Serial.println("Frontal obstacle detected");
  }
  else if (Distances[1] == 0 && Distances[0] < 300 && Distances[2] < 300) {
    Serial.println("Hallway Detected");
  }
  else if (Distances[0]*Distances[1]*Distances[2]) {
    Serial.println("No Obstacles Detected");
  }
}

void followWall(float kp) {
  int sensorSum;
  float error;
  while (sensorSum > 0) {
    sensorSum = 0;
    for (int i = 0; i < 5; i++) {
      sensorSum += digitalRead(digitalSensors[i]);
    }
    Distances[2] = measureDistance(1, 165, 3);
    Distances[0] = measureDistance(1, 15, 3);
    error = (Distances[0] - Distances[2])*kp;
    moveLineOL(1,120+error,120-error,30);
  }
}

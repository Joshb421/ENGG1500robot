#include <Servo.h>
#include "robot.h"
Servo USServo;

//Constants
const int sensor[4] = {A3, A2, A1, A0}; //Pin allocations for the sensors
const float sensorX[4] = {22.5, 7.5, -7.5, -22.5}; //Location of the sensors
const int digitalSensors[5] = {A4, 7, 3, 2, A5}; //The 5 sensors used to track the line status
const int minVals[4] = {24, 25, 23, 25}; //The minimum values for each sensor, when on white paper, see calibration.ino
const int maxVals[4] = {173, 364, 139, 414}; //The maximum values for each sensor, when on a line, see calibration.ino

float Distances [3] = {0, 0, 0}; //Used to track the measuremnts from the US sensor
bool lineHistory[5][20]; //Array used to track the history of the line following, to backtrack if need be
int historyPtr = 0;

//This function simply calls other functions needed for the init process
void robotInit() {
  Serial.begin(57600);
  motorInit();//Motors
  sonarInit();//Sonar sensor
  lineInit();//Line sensor
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
  else{
    lineLostCount; //Resets the counter if a line is detected
  }
  if (lineLostCount > 5) { //Changes strategies if no line is detected 5 times in a row
    Serial.println("Line has been lost too many times, changing strategies...");
    lineLostCount = 0;
    lineLost();
  }
  return (num / den);
}

void lineLost() {
  //Checks the distances from any obstacles left, right and centre
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
    followWall(1.5);
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
    moveLineOL(1,150+error,150-error,200);
  }
}

volatile byte rightStepsToGo;
volatile byte leftStepsToGo;
volatile byte rightCounter;
volatile byte leftCounter;
volatile bool rightFlag;
volatile bool leftFlag;
float pulsesPerMM = 0.09794150344;
float error; 


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
bool moveDone(bool left, bool right) {
  leftCounter = !left*leftCounter; //Resets counter if move done is called
  leftCounter = !right*leftCounter; //Resets counter if move done is called
  if(left){
    analogWrite(5,0);
  }
  if (right){
    analogWrite(5,0);
  }
  return left*right;
}

void moveLine(bool dir, byte duty, int distanceInmm){
  rightCounter = 0;
  leftCounter = 0;
  rightCounter = distanceInmm*pulsesPerMM;
  leftCounter = distanceInmm*pulsesPerMM;
  error += distanceInmm-rightCounter/pulsesPerMM;
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(2), LeftISR, FALLING);//Left encoder
  attachInterrupt(digitalPinToInterrupt(3), RightISR, FALLING);//Right encoder
}

void loop() {
  // put your main code here, to run repeatedly:

}

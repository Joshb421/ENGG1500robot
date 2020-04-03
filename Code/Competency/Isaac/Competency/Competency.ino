#include <ENGG1500Lib.h>

#define  Nill00         0 //Pinouts as recorded in Pinouts.txt
#define  Nill01         1
#define  EncoderLeft    2
#define  EncoderRight   3
#define  Nill04         4
#define  MotorEnableB   5
#define  MotorEnableA   6
#define  IRDigitalOut   7
#define  MotorIn1       8
#define  MotorIn2       9
#define  MotorIn3       10
#define  MotorIn4       11
#define  USEcho         12
#define  USTrigger      13

#define  RangeMax       150 // Range away from the wall where the robot will turn
#define  RangeMin       50 // Too close and it will back up, too far and it will move forward

int running = 1;

enum State {  // Using a basic state machine, each state runs a specific function, the state changes base on input
  STOP,
  WALKFRONT,
  WALKBACK,
  TURN
};

int currentState = STOP;

void setup() {
  // put your setup code here, to run once:


  Serial.begin(9600);
  motorInit();
  sonarInit();
  IRInit();

  currentState = WALKFRONT;
}

void loop() {
  if (running) {
    switch (currentState) { //will run code within States.ino based on the current states
      case STOP:
      stopState();
      break;
      
      case WALKFRONT:
      walkForwardState();
      break;
      
      case WALKBACK:
      walkBackwardState();
      break;
      
      case TURN:
      turnState();
      break;
    }
  
    float d = measureDistance(10);
    if (d > RangeMin && d < RangeMax){
      currentState = TURN; //Turns when withon the specified range value
    }
  
    if (digitalRead(IRDigitalOut)) {
      currentState = STOP; //Will stop when senses black
    }
  }
}

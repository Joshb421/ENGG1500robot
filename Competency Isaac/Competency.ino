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

#define  RangeMax       150
#define  RangeMin       50

int running = 1;

enum State {
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
    // put your main code here, to run repeatedly:
    switch (currentState) {
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
      currentState = TURN;
    }
  
    if (digitalRead(IRDigitalOut)) {
      currentState = STOP;
    }
  }
}

enum State {  // Using a basic state machine, each state runs a specific function, the state changes base on input
  STOP,
  WALKFRONT,
  WALKBACK,
  TURNRIGHT,
  TURNLEFT,
};

State state = STOP;
int stillRunning = 1;

void setup() {
  Serial.begin(57600);
}

void loop() {
  while (stillRunning) {
    switch (state) {
      case STOP:
        state_STOP();
        break;
      case WALKFRONT:
        state_WALKFRONT();
        break;
       case WALKBACK:
        state_WALKBACK();
        break;
      case TURNRIGHT:
        state_TURNRIGHT();
        break;
      case TURNLEFT:
        state_TURNLEFT();
        break;
    }
  }
}

void state_STOP() {
  Serial.println("Stopping");
  stillRunning = 0;
}

void state_WALKFRONT() {
  
}

void state_WALKBACK() {
  
}

void state_TURNRIGHT() {
  
}

void state_TURNLEFT() {
  
}

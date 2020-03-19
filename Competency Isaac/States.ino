void stopState() {
  moveLine(1, 127, 60, 50); //When detecting black, move forward slghtly and stop the program
  running = 0;
}

void walkForwardState() {
  moveLine(1, 127, 60, 50);
  float d = measureDistance(10);
  if (d < RangeMin) {
    currentState = WALKBACK;
  }
}

void walkBackwardState() {
  moveLine(0, 127, 60, 50);
  float d = measureDistance(10);
  if (d > RangeMax) {
    currentState = WALKFRONT;
  }
}

void turnState() {
  moveArc(1, 100, 75, 100);
  currentState = WALKFRONT;
}

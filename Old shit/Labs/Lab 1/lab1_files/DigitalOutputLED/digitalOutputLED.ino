/*
Blink.ino − Blinks the onboard LED at 0.5Hz (once every 2 seconds).
*/
// The setup function runs once when you press reset or power on the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}
// the loop function runs over and over again forever
void loop(){
  digitalWrite(13, HIGH); // turn the LED (pin 13) on, HIGH is +5V
  delay(1000); // wait for a second (1000 ms)
  digitalWrite(13, LOW); // turn the LED (pin 13) off by making the voltage 0V
  delay(1000); // wait for a second
}


//Configures motor control pins, MUST be used before calling moveLine() or moveArc()
void motorInit(){
  float pulsesPerMM = 0.09794150344; //Calclated using encoderRes/pi*diamater
  bool dir = 1; //Sets Default direction to forward
  pinMode(5,OUTPUT); //set ENA as an output
  pinMode(6,OUTPUT); //set ENB as an output
  pinMode(8,OUTPUT); //set IN1 as an output
  pinMode(9,OUTPUT); //set IN2 as an output
  pinMode(10,OUTPUT); //set IN3 as an output
  pinMode(11,OUTPUT); //set IN4 as an output
  enc_init();
  Serial.println("Motors are ready");
}

//Function to move one or both motors at a set speed and distance, must be used after motorInit() is called in setup()
void moveLine(bool dir, byte duty, int distance, int wait)
{
  enc_clear();
  analogWrite(5,duty); //Set Motor speeds
  analogWrite(6,duty);
  digitalWrite(8,dir); //Set polarity for the motors
  digitalWrite(9,!dir); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(10,dir);
  digitalWrite(11,!dir);
  while ((enc_getLeft()+enc_getRight()) / 2.0f < (pulsesPerMM * distance)){
  //Do nothing until exit condition is met (motors have moved to the desired location)
  }
  analogWrite(5,0); //Turn the motors off
  analogWrite(6,0);
  delay(wait); //Wait a certain of time, if requested
}

//Configures variables for the ultrasonic sensor, should be called in setup()
void sonarInit(){
	byte TRIG = 13;
	byte ECHO = 12;
	pinMode(TRIG, OUTPUT);
	const float speed_sound = 340.29;
	Serial.println("Ultrasonic sensor is ready");

}

//Returns a distance, as measured by the ultrasonic sensor, can be sampled mutiple times to increase accuracy
unsigned float measureDistance(int samples) {
  long duration = 0;
  float sum;
  for(int n = 0; n<samples;n++){ //Takes a number of samles to reduce hysterisis
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    duration = pulseIn(ECHO, HIGH); 
    sum = sum + (0.5 * duration * 1e-6 * speed_sound * 1e3);
  }
  return sum/samples;
}


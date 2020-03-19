//Configures motor control pins, MUST be used before calling moveLine() or moveArc()
#define PPM 0.09794150344f //Calclated using encoderRes/pi*diamater
#define Mach1 340.29f //Speed of sound



#define  Direction            1 //Defult direction of motion, if polarity of motors changes update this value

void motorInit(){
  pinMode(MotorEnableA,OUTPUT); //set ENA as an output
  pinMode(MotorEnableB,OUTPUT); //set ENB as an output
  pinMode(MotorIn1,OUTPUT); //set IN1 as an output
  pinMode(MotorIn2,OUTPUT); //set IN2 as an output
  pinMode(MotorIn3,OUTPUT); //set IN3 as an output
  pinMode(MotorIn4,OUTPUT); //set IN4 as an output
  enc_init();
  //Serial.println("Motors are ready");
}

//Function to move one or both motors at a set speed and distance, must be used after motorInit() is called in setup()
void moveLine(bool dir, byte duty, int distance, int wait) {
  enc_clear();
  int done = 0;
  analogWrite(MotorEnableA,duty); //Set Motor speeds
  analogWrite(MotorEnableB,duty);
  digitalWrite(MotorIn1,dir); //Set polarity for the motors
  digitalWrite(MotorIn2,!dir); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(MotorIn3,dir);
  digitalWrite(MotorIn4,!dir);
  int timer = 0;
  while (!done || timer){
    if (enc_getLeft() > PPM * distance) {
      analogWrite(MotorEnableA,0);
    }
    if (enc_getRight() > PPM * distance) {
      analogWrite(MotorEnableB,0);
    }
    if (enc_getLeft() > PPM * distance && enc_getRight() > PPM * distance) {
      done = 1;
    }
    
    //Do nothing until exit condition is met (motors have moved to the desired location)
  }

  delay(wait); //Wait a certain of time, if requested
}

void moveArc(bool dir, byte duty, int distance, int wait) {
  enc_clear();
  int done = 0;
  analogWrite(MotorEnableA,duty); //Set Motor speeds
  analogWrite(MotorEnableB,duty);
  digitalWrite(MotorIn1,dir); //Set polarity for the motors
  digitalWrite(MotorIn2,!dir); //Dir is inverted (!) to create a voltage differnetial
  digitalWrite(MotorIn3,!dir);
  digitalWrite(MotorIn4,dir);
  while (!done){
    if (enc_getLeft() > PPM * distance) {
      analogWrite(MotorEnableA,0);
    }
    if (enc_getRight() > PPM * distance) {
      analogWrite(MotorEnableB,0);
    }
    if (enc_getLeft() > PPM * distance && enc_getRight() > PPM * distance) {
      done = 1;
    }
    
    //Do nothing until exit condition is met (motors have moved to the desired location)
  }
  delay(wait); //Wait a certain of time, if requested
}

//Configures variables for the ultrasonic sensor, should be called in setup()
void sonarInit(){
	pinMode(USTrigger, OUTPUT);
	//Serial.println("Ultrasonic sensor is ready");
}

//Returns a distance, as measured by the ultrasonic sensor, can be sampled mutiple times to increase accuracy
float measureDistance(int samples) {
  long duration = 0;
  float sum = 0; //Must be reinitialised as memory location same as previous call
  for(int n = 0; n < samples; n++){ //Takes a number of samles to reduce hysterisis
    digitalWrite(USTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(USTrigger, LOW);
    duration = pulseIn(USEcho, HIGH); 
    sum = sum + (0.5 * duration * 1e-6 * Mach1 * 1e3);
  }
  return sum/samples;
}

void IRInit(){
  pinMode(IRDigitalOut, INPUT);
}

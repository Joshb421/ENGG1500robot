// These #define statements make the code more readable.
// Instead of a pin number "7" or "12" we can write "ECHO" or "TRIG"
#define ECHO 12
#define TRIG 7

void setup() {
  pinMode(ECHO,INPUT); //Initialise pin 12 as an input
  pinMode(TRIG,OUTPUT); //Initialise pin 7 as an output
  Serial.begin(9600); //begin serial communication 
}

void loop() {
  unsigned int distance_mm = 0; //This variable will hold the distance
  distance_mm = sonar_mm(); //call the function sonar_mm and store the result in distance_mm
  Serial.print("Distance="); //print the result to the serial monitor
  Serial.println(distance_mm);
}

unsigned int sonar_mm(void){
  long duration = 0;
  const float speed_sound = 340.29;// m/s, "const" makes the compiler able to optimise the program where this variable is used, cool!
  // Read in a distance from the ultrasonic distance sensor:
  // The ultrasonic burst is triggered by a HIGH pulse of 10 microseconds.
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  //read length of time pulse
  duration = pulseIn(ECHO, HIGH); //This function measures a pulsewidth and returns the width in microseconds
  // convert the time into a distance
  // the code "(unsigned int)" turns the result of the distance calculation
  // into an integer instead of a floating point (decimal or fractional) number.
  return (unsigned int)(0.5 * duration * 1e-6 * speed_sound * 1e3); 
  //"unsigned" ensures we are returning an unsigned number, remember that there is no such thing as negative distance.
}
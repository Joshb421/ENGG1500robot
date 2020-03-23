//Distances from the centroid of the robot to the centre of each sensor in mm 
float x1 = -22.5;
float x2 = -7.5;
float x3 = 7.5;
float x4 = 22.5;

//Variables to store each data point in
float w1;
float w2;
float w3;
float w4;

//Variables for storing the numerator and denominator of Equation 1
float den = 0;   
float num = 0;

void setup() {
  // Initialise pins for line sensors
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  Serial.begin(115200);
}

void loop() {
  //  TODO: Take sensor measurements using "w1 = analogRead(pin)" (store sensor data in w1, w2, w3, w4)
  //  TODO: Calculate numerator of weighted average, store in num
  //  TODO: Calculate denominator of weighted average, store in den

  float lineDist = num/den;

  Serial.print("Distance from line = ");
  Serial.println(lineDist);
  delay(50);  //  50ms delay
}

const int sensor[4] = {A3, A2, A1, A0};
const float sensorX[4] = {22.5f, 7.5f, -7.5f, -20.0f};


float weightedAverage() {
  float num = 0;
  float den = 0;
  for (int i = 0; i < 4; i++) {
    int value = analogRead(sensor[i]);
    num += value * sensorX[i];
    den += value;
  }
  return (num / den);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  for (int i = 0; i < 4; i++) {
    Serial.println(sensor[i]);
  }
  Serial.println();
  
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println(weightedAverage()); 

}

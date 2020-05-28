const int sensor[4] = {A3, A2, A1, A0};
int minVals[4]={4000,4000,4000,4000};
int maxVals[4]={0,0,0,0};
int value;
void setup() {
  Serial.begin(57600);
}

void loop() {
  for (int i = 0;i<4;i++){
    value=analogRead(sensor[i]);
    if (value<minVals[i]){
      minVals[i]=value;
    }
    if (value>maxVals[i]){
      maxVals[i]=value;
    }
    Serial.print("const int minVals[4]={");
    Serial.print(minVals[0]);
    Serial.print(',');
    Serial.print(minVals[1]);
    Serial.print(',');
    Serial.print(minVals[2]);
    Serial.print(',');
    Serial.print(minVals[3]);
    Serial.println("};");
    Serial.print("const int maxVals[4]={");
    Serial.print(maxVals[0]);
    Serial.print(',');
    Serial.print(maxVals[1]);
    Serial.print(',');
    Serial.print(maxVals[2]);
    Serial.print(',');
    Serial.print(maxVals[3]);
    Serial.println("};");
  }
}

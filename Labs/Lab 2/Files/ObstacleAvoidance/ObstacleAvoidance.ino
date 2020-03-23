//START
void setup() {
  // put your setup code here, to run once:
  pinMode(5,OUTPUT); //set ENA as an output
  pinMode(6,OUTPUT); //set ENB as an output
  pinMode(8,OUTPUT); //set IN1 as an output
  pinMode(9,OUTPUT); //set IN2 as an output
  pinMode(10,OUTPUT); //set IN3 as an output
  pinMode(11,OUTPUT); //set IN4 as an output
  delay(5000);//delay before starting loop
}

void loop() {
  // put your main code here, to run repeatedly:
  leftForwards();  //set the left motor to run forwards
  rightForwards();  //set the right motor to run forwards
  int sensorVal = digitalRead(12);    //1 if no obstacle is present otherwise 0

  //let's say our desired speeds are 255 and 180
  //multiply the previous by the desired speed (* is multiply) 
  analogWrite(5,sensorVal*255); //circle
  analogWrite(6,sensorVal*180); //circle
}

void leftForwards(void)
{
  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
}

void leftBackwards(void)
{
  digitalWrite(8,HIGH);
  digitalWrite(9,LOW);
}

void rightForwards(void)
{
  digitalWrite(10,LOW);
  digitalWrite(11,HIGH);
}
void rightBackwards(void)
{
  digitalWrite(10,HIGH);
  digitalWrite(11,LOW);
}
//END
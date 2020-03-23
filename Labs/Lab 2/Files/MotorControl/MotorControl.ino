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
  leftForwards();	//set the left motor to run forwards by calling the function leftForwards, listed below
  rightForwards();	//set the right motor to run forwards by calling the function rightForwards,listed below
  analogWrite(5,200);	//set the left motor to half speed (ish)
  analogWrite(6,200);	//set the right motor to half speed (ish)
  delay(5000);	//allow the motors to run forwards for 5000 ms (5 seconds)
  analogWrite(5,0);	//stop the left motor
  analogWrite(6,0);	//stop the right motor
  delay(5000);	//stop the motors for 5 seconds
  
  //TODO: set both motor directions to backwards
  //TODO: set the speed of both motors to full speed
  //TODO: insert a delay to allow the motors to run backwards for 2.5 seconds before this loop repeats
}

void leftForwards(void) //This function sets IN1 = LOW and IN2 = HIGH in order to set the direction to forwards for motor 1
{
  digitalWrite(8,LOW); //IN1
  digitalWrite(9,HIGH); //IN2
}

void leftBackwards(void) //This function sets IN1 = HIGH and IN2 = LOW in order to set the direction to backwards for motor 1
{
  digitalWrite(8,HIGH); //IN1
  digitalWrite(9,LOW); //IN2
}

void rightForwards(void)  //This function sets IN3 = LOW and IN4 = HIGH in order to set the direction to forwards for motor 2
{
  digitalWrite(10,LOW); //IN3
  digitalWrite(11,HIGH); //IN4
}
void rightBackwards(void) //This function sets IN3 = HIGH and IN4 = LOW in order to set the direction to forwards for motor 2
{
  digitalWrite(10,HIGH); //IN3
  digitalWrite(11,LOW); //IN4
}
//END
//START
#include <ENGG1500_lib.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(5,OUTPUT); //set ENA as an output
  pinMode(6,OUTPUT); //set ENB as an output
  pinMode(8,OUTPUT); //set IN1 as an output
  pinMode(9,OUTPUT); //set IN2 as an output
  pinMode(10,OUTPUT); //set IN3 as an output
  pinMode(11,OUTPUT); //set IN4 as an output
  enc_init();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  enc_clear();//set both encoder counts to 0
  leftForwards();//set motor directions to forwards
  rightForwards();
  
  while((enc_getLeft()+enc_getRight())/2 < 50){
    analogWrite(5,255);
    analogWrite(6,255);
  }
  
  analogWrite(5,0);
  analogWrite(6,0);
  delay(200);
  enc_clear();
  leftBackwards();
  rightBackwards();
 
  while((enc_getLeft()+enc_getRight())/2 < 50){
    analogWrite(5,200);
    analogWrite(6,200);
  }
  delay(200);
  
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
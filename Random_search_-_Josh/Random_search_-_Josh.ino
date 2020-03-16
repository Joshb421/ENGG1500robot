#include <ENGG1500Lib.h>
#define ECHO 12
#define TRIG 13




const float pulsesPerMM = 0.09794150344f;
bool dir = 1;
int randomNum;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  motorInit();
  delay(2000);
  randomSeed(analogRead(0));
  pinMode(ECHO, INPUT); //Initialise pin 12 as an input
  pinMode(TRIG, OUTPUT); //Initialise pin 7 as an output
  
}

void loop() {

  while (digitalRead(7)) {
    // put your main code here, to run repeatedly:
    enc_clear();//set both encoder counts to 0
    if (sonar_mm() > 70) {
      move(dir, 120, 120, 5, 0);
    }
    else if (sonar_mm() < 70) {
      randomNum = random(5,120);
      move(!dir, 120, 120, 100, 0);
      move(dir, 0, 120, randomNum, 0);
    }
    else if (sonar_mm() < 40 && sonar_mm() > 0) {
      move(!dir, 100, 100, 5, 0);
    }
  }
}

void motorInit() {
  pinMode(5, OUTPUT); //set ENA as an output
  pinMode(6, OUTPUT); //set ENB as an output
  pinMode(8, OUTPUT); //set IN1 as an output
  pinMode(9, OUTPUT); //set IN2 as an output
  pinMode(10, OUTPUT); //set IN3 as an output
  pinMode(11, OUTPUT); //set IN4 as an output
  enc_init();
}
bool move(bool dir, byte dutyLeft, byte dutyRight, int distance, int wait)
{
  enc_clear();
  analogWrite(5, dutyLeft);
  analogWrite(6, dutyRight);
  digitalWrite(8, dir);
  digitalWrite(9, !dir);
  digitalWrite(10, dir);
  digitalWrite(11, !dir);
  Serial.println("Test");
  while ((enc_getLeft() + enc_getRight()) / 2.0f < (pulsesPerMM * distance)) {
    Serial.println(enc_getLeft()); //43332
  }
  analogWrite(5, 0);
  analogWrite(6, 0);

  delay(wait);
}

unsigned int sonar_mm(void) {
  long duration = 0;
  const float speed_sound = 340.29;
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  return (unsigned int)(0.5 * duration * 1e-6 * speed_sound * 1e3);
}


//END

//Digital Input IR
void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT); //Initialising pin A0 as an input
  Serial.begin(9600);//Setting up serial communication to view results,
  //9600 is a common and default baud rate,
  //Ctrl+Shift+M to open the serial monitor.
}

//Declaring a variable "sensorReading" that we can use later,
//At the same time we declare it we set it equal to 0,
//It is declared here so that it can be used inside functions such as void loop()


int sensorReading = 0;
void loop() {
  // put your main code here, to run repeatedly:
  
  //TODO: Insert 100ms delay here.

  //TODO: Read the sensor into "sensor" here.

  //Print the value read to the serial monitor.
  Serial.println(sensorReading);//Note that an l(L),1,I all look similar in code font
  //Extended: Set the LED on the board to the value read from the sensor
  //using digitalWrite.
}


#include <Servo.h>
Servo sillyServo;

String inputString = "";
bool stringComplete = false;
int sillyPin = 9;
int servoOff = 150;
int servoOn = 80;

void setup() {

  sillyServo.attach(sillyPin);
  sillyServo.write(servoOff); 
  
  Serial.begin(9600);
  
  inputString.reserve(200);
}

void loop() {
  if (stringComplete) {
      inputString.trim();
      if(inputString == "launch"){
        launch();
      }
    inputString = "";
    stringComplete = false;
  }
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    
    inputString += inChar;
    
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void launch(){
  sillyServo.write(servoOn);
  delay(500); // length of time silly can is presses
  sillyServo.write(servoOff);
}

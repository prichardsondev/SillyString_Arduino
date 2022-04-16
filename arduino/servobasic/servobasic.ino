#include <Servo.h>
Servo sillyServo;

void setup() {
  // put your setup code here, to run once:
  sillyServo.attach(9);
  sillyServo.write(175);
}

void loop() {
  // put your main code here, to run repeatedly:

}

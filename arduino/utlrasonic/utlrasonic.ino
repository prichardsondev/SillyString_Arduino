const int TRIG_PIN = 8;
const int ECHO_PIN = 7;


// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

void setup() {

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, HIGH);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}

void loop() {

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == LOW );

  unsigned long t1 = micros();
  while ( digitalRead(ECHO_PIN) == HIGH);
  unsigned long t2 = micros();
  unsigned long pulse_width = t2 - t1;

  float inches = pulse_width / 148.0;
  
  Serial.print(inches);
  Serial.println(" inches");

  delay(50);
}

#include <Servo.h>
Servo sillyServo;

String inputString = "";
bool stringComplete = false;
int sillyPin = 9;
int servoOff = 150;
int servoOn = 80;

#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

#define DHTTYPE DHT11
#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);

#define TRIG_PIN 8
#define ECHO_PIN 7

float lastTemp = 0;
float lastDistance = 0;
bool writeSerial = false;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

//create non blocking io check temp and distance every .1 seconds
unsigned long interval = 100;
unsigned long previousMillis = 0;

void setup() {

  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, HIGH);
  
  sillyServo.write(servoOff);
  sillyServo.attach(sillyPin);
  
  Serial.begin(9600);

  inputString.reserve(200);

  dht.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if (stringComplete) {
    inputString.trim();
    if (inputString == "launch") {
      launch();
    }
    inputString = "";
    stringComplete = false;
  }

  if (currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;
    // Hold the trigger pin high for at least 10 us
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Wait for pulse on echo pin
    while ( digitalRead(ECHO_PIN) == 0 );

    // Measure how long the echo pin was held high (pulse width)
    // Note: the micros() counter will overflow after ~70 min
    unsigned long t1 = micros();
    while ( digitalRead(ECHO_PIN) == 1);
    unsigned long t2 = micros();
    unsigned long pulse_width = t2 - t1;

    float inches = pulse_width / 148.0;
    //only write distance to serial if change >= .5
    if (abs(inches - lastDistance) >= .5) {
      lastDistance = inches;
      writeSerial = true;
    }

    //Temp
    // Read temperature as Celsius (the default)
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float fahrenheit = dht.readTemperature(true);

    //only write temp to serial if change >= .5
    if (abs(fahrenheit - lastTemp) >= .5) {
      lastTemp = fahrenheit;
      writeSerial = true;
    }

    if (writeSerial) {
      writeJson(fahrenheit, inches);
      writeSerial = false;
    }
  }
}

void writeJson(float fahrenheit, float inches) {
  const size_t capacity = JSON_OBJECT_SIZE(4) + 30;
  DynamicJsonDocument doc(1024);

  doc["fahrenheit"] = fahrenheit;
  doc["inches"] = inches;

  serializeJson(doc, Serial);
  Serial.println();
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

void launch() {
  sillyServo.write(servoOn);
  delay(500); // length of time silly can is presses
  sillyServo.write(servoOff);
}

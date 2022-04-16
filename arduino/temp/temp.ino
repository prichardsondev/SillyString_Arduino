/*
 * Hardware Connections:
 *  Arduino | DHT11 
 *  -------------------
 *    5V    |   +     
 *    2     |   Out     
 *    GND   |   -
 */

#include "DHT.h"

DHT dht(2, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float f = dht.readTemperature();
  Serial.print("Temp Celsius: ");
  Serial.println(f);
  delay(1000);
}
//Nick Sankovich.
//Return true if the sensor is active, false otherwise. There must be two modes "ANALOG", and "DIGITAL". In ANALOG mode, the sensor is active if the value read from the sensor is above a certain threshold. In DIGITAL mode, the sensor is active if the value read from the sensor is HIGH. The mode and threshold should be configurable in the constructor of the Sensor class.

#include "Sensor.h"
#include <Arduino.h>

#define SENSOR_ACTIVE_HIGH 0
#define SENSOR_MODE "ANALOG"
#define SENSOR_THRESHOLD 512
#define SENSOR_PIN_ANALOG A0
#define SENSOR_PIN_DIGITAL 13
Sensor::Sensor() {}

void Sensor::begin() {
  if (strcmp(SENSOR_MODE, "DIGITAL") == 0) {
    pinMode(SENSOR_PIN_DIGITAL, INPUT_PULLUP);
  } else {
    pinMode(SENSOR_PIN_ANALOG, INPUT_PULLUP);
  }
}

void Sensor::update() {}

bool Sensor::isActive() {
  if (strcmp(SENSOR_MODE, "DIGITAL") == 0) {
  int value = digitalRead(SENSOR_PIN_DIGITAL);

    if (SENSOR_ACTIVE_HIGH) {
      return value == HIGH;
    } else{
      return value == LOW;
    }
  }
  if (strcmp(SENSOR_MODE, "ANALOG") == 0){
    int value = analogRead(SENSOR_PIN_ANALOG);

    Serial.print("Analog value: ");
    Serial.println(value);

    if (SENSOR_ACTIVE_HIGH) {
      return value >= SENSOR_THRESHOLD;
    }  else{
        return value <= SENSOR_THRESHOLD;
    }
  }
  return false;
}


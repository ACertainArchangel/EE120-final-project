//Nick Rutz.
//Return true if the sensor is active, false otherwise. There must be two modes "ANALOG", and "DIGITAL". In ANALOG mode, the sensor is active if the value read from the sensor is above a certain threshold. In DIGITAL mode, the sensor is active if the value read from the sensor is HIGH. The mode and threshold should be configurable in the constructor of the Sensor class.

#include "Sensor.h"
#include <Arduino.h>

#define SENSOR_ACTIVE_HIGH 1 
#define SENSOR_MODE "DIGITAL"
#define SENSOR_THRESHOLD 512
#define SENSOR_PIN_ANALOG A0
#define SENSOR_PIN_DIGITAL 12
Sensor::Sensor() {}

void Sensor::begin() {}

void Sensor::update() {}

bool Sensor::isActive() {
  if (SENSOR_MODE == "DIGITAL") {
  int value = digitalRead(SENSOR_PIN_DIGITAL);

    if (SENSOR_ACTIVE_HIGH) {
      return value == HIGH;
    } else{
      return value == LOW;
    }
  }
  if (SENSOR_MODE == "ANALOG"){
    int value = analogRead(SENSOR_PIN_ANALOG);

    if (SENSOR_ACTIVE_HIGH) {
      return value >= SENSOR_THRESHOLD;
    }  else{
        return value <= SENSOR_THRESHOLD;
    }
  }
  return false;
}


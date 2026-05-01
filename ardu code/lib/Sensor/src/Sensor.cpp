//Nick Rutz.
//Return true if the sensor is active, false otherwise. There must be two modes "ANALOG", and "DIGITAL". In ANALOG mode, the sensor is active if the value read from the sensor is above a certain threshold. In DIGITAL mode, the sensor is active if the value read from the sensor is HIGH. The mode and threshold should be configurable in the constructor of the Sensor class.

#include "Sensor.h"

Sensor::Sensor() {}

void Sensor::begin() {}

void Sensor::update() {}

bool Sensor::isActive() {
  return false;
}

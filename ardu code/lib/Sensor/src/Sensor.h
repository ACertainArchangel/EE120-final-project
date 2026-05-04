#pragma once
#include <string.h>

#define SENSOR_MODE "ANALOG" // "DIGITAL" or "ANALOG"
#define SENSOR_THRESHOLD 512 // Only used in ANALOG mode, the sensor is active if the value read from the sensor is above this threshold.

class Sensor {
 public:
  Sensor();

  void begin();
  void update();
  bool isActive();
};

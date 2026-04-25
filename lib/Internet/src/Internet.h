#pragma once
#include <Arduino.h>

#define NUM_LIGHTS 10 // HACK: This is defined in both Internet.h and SignalCoordinator.h so if you change one change the other.

class Internet {
 public:
  Internet();

  void begin();
  void update();
  void txMessage(byte target_number);
  void rxMessage(byte array[], int* size);
};

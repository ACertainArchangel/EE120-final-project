#pragma once
#include "Internet.h"
#include "Utils.h"
#include <Arduino.h>

#define NUM_LIGHTS 10 // HACK: This is defined in both Internet.h and SignalCoordinator.h so if you change one change the other.

class SignalCoordinator {
  public:
  SignalCoordinator();

  void begin();
  void update();
  bool rxActiveSignal(byte lamp_number);
  void txActiveSignal(byte lamp_number);
};

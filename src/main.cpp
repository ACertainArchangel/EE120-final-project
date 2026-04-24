#include <Arduino.h>

#include "Internet.h"
#include "SignalCoordinator.h"
#include "Light.h"
#include "Sensor.h"

Internet internet;
SignalCoordinator signalCoordinator;
Light light;
Sensor sensor;



void setup() {
  internet.begin();
  signalCoordinator.begin();
  light.begin();
  sensor.begin();
}

void loop() {
  internet.update();
  signalCoordinator.update();
  light.update();
  sensor.update();
}

int main() {
  init();
  setup();
  while (true) {
    loop();
  }
  // Never happens
  return 0;
}
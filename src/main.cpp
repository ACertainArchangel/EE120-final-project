#include <Arduino.h>

#include "Internet.h"
#include "SignalCoordinator.h"
#include "Light.h"
#include "Sensor.h"

//NUM_LIGHTS is defined in SignalCoordinator.h and in Internet.h so if you change one change the other... #HACK

Internet internet;
SignalCoordinator signalCoordinator;
Light light;
Sensor sensor;

byte lamp_number = 0;
byte color_r = 255;
byte color_g = 255;
byte color_b = 255;

void setup() {

  internet.begin();
  signalCoordinator.begin();
  light.begin();
  sensor.begin();

}

byte seconds_active = 0;
unsigned long int last_update_time = millis();

void loop() {

  internet.update();
  signalCoordinator.update();
  light.update();
  sensor.update();

  if (sensor.isActive()){
    seconds_active = 60;
    signalCoordinator.txActiveSignal(lamp_number);
    last_update_time = millis();
  }
  else if (signalCoordinator.rxActiveSignal(lamp_number)){
    seconds_active = 60;
    last_update_time = millis();
  }
  else if (seconds_active > 0 && millis() - last_update_time > 1000){
    seconds_active--;
    last_update_time = millis();
  }

  if (seconds_active > 0){
    light.setColor(color_r, color_g, color_b);
  }
  else{
    light.setColor(0, 0, 0);
  }

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
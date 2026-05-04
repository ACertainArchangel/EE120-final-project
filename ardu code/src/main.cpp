#include <Arduino.h>

#include "Internet.h"
#include "SignalCoordinator.h"
#include "Light.h"
#include "Sensor.h"

//NUM_LIGHTS is defined in SignalCoordinator.h and in Internet.h so if you change one change the other... #HACK

#define LAMP_INDEX 2
#define RED 255
#define GREEN 255
#define BLUE 255

#define ARDU_TX_ADD_PIN 12
#define ARDU_TX_REMOVE_PIN 5

#define TIME 10

Internet internet;
SignalCoordinator signalCoordinator;
Light light;
Sensor sensor;

byte lamp_number = LAMP_INDEX;
byte color_r = RED;
byte color_g = GREEN;
byte color_b = BLUE;

byte seconds_active = 0;
unsigned long int last_update_time = 0;

void setup() {

  Serial.begin(9600);

  internet.begin();
  signalCoordinator.begin(internet);
  light.begin();
  sensor.begin();

  last_update_time = millis();
}

void loop() {

  internet.update();
  signalCoordinator.update();
  light.update();
  sensor.update();

  if (sensor.isActive()){
    seconds_active = TIME;
    signalCoordinator.txActiveSignal();
    last_update_time = millis();
    Serial.println("Sensor active");
  }
  else if (signalCoordinator.rxActiveSignal(lamp_number)){
    seconds_active = TIME;
    last_update_time = millis();
    Serial.println("Received active signal");
    internet.txDeleteMessage();
  }
  else if (seconds_active > 0 && millis() - last_update_time > 1000){
    seconds_active--;
    last_update_time = millis();
    Serial.print("Seconds active: ");
    Serial.println(seconds_active);
  }

  if (seconds_active > 0){
    light.setColor(color_r, color_g, color_b);
    Serial.println("Light on");
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
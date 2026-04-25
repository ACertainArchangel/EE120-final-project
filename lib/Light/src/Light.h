#pragma once
#include <Arduino.h>

#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

class Light {
 public:
  Light();

  void begin();
  void update();
  void setColor(byte r, byte g, byte b);
};

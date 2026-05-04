#pragma once
#include <Arduino.h>

#define RED_PIN 10
#define GREEN_PIN 11
#define BLUE_PIN 3

class Light {
 public:
  Light();

  void begin();
  void update();
  void setColor(byte r, byte g, byte b);
};

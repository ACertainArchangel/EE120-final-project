#pragma once
#include <Arduino.h>

class Light {
 public:
  Light();

  void begin();
  void update();
  void setColor(byte r, byte g, byte b);
};

//Nick Sankovich
//Light. Set the color.

#include "Light.h"

Light::Light() {}

void Light::begin() {
  pinMode(RED_PIN, OUTPUT);  //Set RGB pins as outputs
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}
  
void Light::update() {}

void Light::setColor(byte r, byte g, byte b) {
  analogWrite(RED_PIN, r); //Set color intensities
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);

}

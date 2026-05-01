#pragma once
#include <Arduino.h>

#define NUM_LIGHTS 4 // HACK: This is defined in both Internet.h and SignalCoordinator.h so if you change one change the other.

#define ARDU_TX_ADD_PIN 2
#define ARDU_TX_REMOVE_PIN 5

#define ARDU_RX_PIN1 3
#define ARDU_RX_PIN2 3
#define ARDU_RX_PIN3 3
#define ARDU_RX_PIN4 3


class Internet {
 public:
  Internet();

  void begin();
  void update();
  void txMessage(byte target_number);
  void rxMessage(byte array[], int* size);

 private:
  static unsigned long started_message_at;
  static bool is_sending_message;
};

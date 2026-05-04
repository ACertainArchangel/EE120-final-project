#pragma once
#include <Arduino.h>

#define NUM_LIGHTS 4 // HACK: This is defined in both Internet.h and SignalCoordinator.h so if you change one change the other.

#define ARDU_TX_ADD_PIN 12
#define ARDU_TX_REMOVE_PIN 5
# define ARDU_RX_PIN 6

class Internet {
 public:
  Internet();

  void begin();
  void update();
  void txMessage();
  bool rxMessage();
  void txDeleteMessage();

 private:
  static unsigned long started_message_at;
  static bool is_sending_message;
  static unsigned long started_delete_message_at;
  static bool is_sending_delete_message;
};

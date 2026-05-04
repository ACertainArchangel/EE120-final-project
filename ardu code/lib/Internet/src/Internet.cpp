//Gabe
//txMessage appends the target_number to the end of the message queue removing duplicates.
//rxMessage copies the message queue into the provided array and sets size to the number of messages in the queue.

#include "Internet.h"

Internet::Internet() {}

unsigned long Internet::started_message_at = 0;
bool Internet::is_sending_message = false;

unsigned long Internet::started_delete_message_at = 0;
bool Internet::is_sending_delete_message = false;

void Internet::begin() {
    pinMode(ARDU_TX_ADD_PIN, OUTPUT);
    pinMode(ARDU_TX_REMOVE_PIN, OUTPUT);
    pinMode(ARDU_RX_PIN, INPUT);
}

void Internet::update() {
  if(is_sending_message and millis() - started_message_at > 1000){
    digitalWrite(ARDU_TX_ADD_PIN, LOW);
    is_sending_message = false;
  }
  if(is_sending_delete_message and millis() - started_delete_message_at > 1000){
    digitalWrite(ARDU_TX_REMOVE_PIN, LOW);
    is_sending_delete_message = false;
  }
}

void Internet::txMessage() {
    if (is_sending_message) return;
    is_sending_message = true;
    digitalWrite(ARDU_TX_ADD_PIN, HIGH);
    started_message_at = millis();
}

bool Internet::rxMessage() {
    return digitalRead(ARDU_RX_PIN) == HIGH;
}

void Internet::txDeleteMessage() {
    if (is_sending_delete_message) return;
    digitalWrite(ARDU_TX_REMOVE_PIN, HIGH);
    is_sending_delete_message = true;
    started_delete_message_at = millis();
}

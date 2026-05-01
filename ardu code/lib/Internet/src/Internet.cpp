//Gabe
//txMessage appends the target_number to the end of the message queue removing duplicates.
//rxMessage copies the message queue into the provided array and sets size to the number of messages in the queue.

#include "Internet.h"

Internet::Internet() {}

unsigned long Internet::started_message_at = millis();
bool Internet::is_sending_message = true;

void Internet::begin() {
    pinMode(ARDU_TX_ADD_PIN, OUTPUT);
    pinMode(ARDU_TX_REMOVE_PIN, OUTPUT);
}

void Internet::update() {
  if(is_sending_message and millis() - started_message_at > 1000){
    digitalWrite(ARDU_TX_ADD_PIN, LOW);
  }
}

void Internet::txMessage(byte target_number) {
    is_sending_message = true;
    digitalWrite(ARDU_TX_ADD_PIN, HIGH);
}

void Internet::rxMessage(byte array[], int* size) {
    if (digitalRead(ARDU_RX_PIN) == HIGH){
        *size = 1;
        array[0] = 0; // HACK: This is a placeholder for the actual message queue which is not implemented.
    }
    else{
        *size = 0;
    }
}


//Gabe
//txMessage appends the target_number to the end of the message queue removing duplicates.
//rxMessage copies the message queue into the provided array and sets size to the number of messages in the queue.

#include "Internet.h"

Internet::Internet() {}

unsigned long Internet::started_message_at = millis();
bool Internet::is_sending_message = true;

unsigned long Internet::started_delete_message_at = millis();
bool Internet::is_sending_delete_message = true;

void Internet::begin() {
    pinMode(ARDU_TX_ADD_PIN, OUTPUT);
    pinMode(ARDU_TX_REMOVE_PIN, OUTPUT);
}

void Internet::update() {
  if(is_sending_message and millis() - started_message_at > 1000){
    digitalWrite(ARDU_TX_ADD_PIN, LOW);
  }
}

void Internet::txMessage() {
    is_sending_message = true;
    digitalWrite(ARDU_TX_ADD_PIN, HIGH);
    started_message_at = millis();
}

void Internet::rxMessage(byte array[], int* size) {
    int i=0;
    if(digitalRead(ARDU_RX_PIN1) == HIGH) {
        array[i] = 1;
        i++;
    }
    if (digitalRead(ARDU_RX_PIN2) == HIGH) {
        array[i] = 2;
        i++;
    }
    if (digitalRead(ARDU_RX_PIN3) == HIGH) {
        array[i] = 3;
        i++;
    }
    if (digitalRead(ARDU_RX_PIN4) == HIGH) {
        array[i] = 4;
        i++;
    }
    *size = i;
}

void Internet::rxDeleteMessage() {
    digitalWrite(ARDU_TX_REMOVE_PIN, HIGH);
    is_sending_delete_message = true;
    started_delete_message_at = millis();
}

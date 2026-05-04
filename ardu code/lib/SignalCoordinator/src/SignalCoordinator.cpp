// Sam Schultz.
// Send signals to the other lights based upon which light it is.
// Use internet.txMessage(target_number) and internet.rxMessage() to send and receive messages.

#include "SignalCoordinator.h"

SignalCoordinator::SignalCoordinator() : internet_ptr(nullptr), prev_in_list(false) {}

void SignalCoordinator::begin(Internet& internet) {
    internet_ptr = &internet;
}

void SignalCoordinator::update() {}

bool SignalCoordinator::rxActiveSignal(byte lamp_number) {
    //int len = 0;
    //byte arr[NUM_LIGHTS];
    //internet_ptr->rxMessage(arr, &len);
    //bool in_list = in_in(arr, len, lamp_number);

    //if (in_list && !prev_in_list) {
    //    internet_ptr->txDeleteMessage();
    //    Serial.println("Received active signal, sending delete message");
    //}
    //prev_in_list = in_list;

    //return in_list;

    return internet_ptr->rxMessage();
}

void SignalCoordinator::txActiveSignal() {
    internet_ptr->txMessage();
}

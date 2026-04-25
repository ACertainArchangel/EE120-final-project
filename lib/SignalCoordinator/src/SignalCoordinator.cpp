// Sam Schultz. 
// Send signals to the other lights based upon which light it is. 
// Use internet.txMessage(target_number) and internet.rxMessage() to send and receive messages.

#include "SignalCoordinator.h"

namespace SignalCoordinator{
    Internet internet;

    byte arr[10];

    void begin() {}

    void update() {}

    bool rxActiveSignal(byte lamp_number) {
        int len = 0;
        byte arr[NUM_LIGHTS];
        internet.rxMessage(arr, &len);
        return in_in(arr, len, lamp_number);
    }

    void txActiveSignal(byte lamp_number) {
        // internet.txMessage(lamp_number); for all the lamp numbers adjacent to our lamp number. 
        // Should be a switch case or generated from some topology map. You'll figure it out :)
    }
}



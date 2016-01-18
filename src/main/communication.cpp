#include <Wire.h>
#include <SPI.h>
#include "mcp_can.h"
#include "CanBus.h"
#include "communication.h"

void initCommunication() {
    Serial.begin(115200);
    CanBus.begin(CAN_5KBPS);

    #ifdef DEBUG
        Serial.println("MotionController com open");
    #endif
}

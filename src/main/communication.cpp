#include "communication.h"
#include "mcp_can.h"
#include "CanBus.h"

ReadState_t currentState     = SIZE;
uint8_t checkXor             = 0;
uint8_t cmd                  = 0;
uint16_t size                = 0;
uint8_t buf[MAX_PACKET_SIZE] = { 0 };
uint8_t bufPos               = 0;

void initCommunication() {
    Serial.begin(115200);
    CanBus.begin(CAN_5KBPS);

    #ifdef DEBUG
        Serial.println("MotionController com open");
    #endif
}

void processCommunication() {
    processUART();
}

void processUART() {
    if (Serial.available() <= 0) {
        return;
    }

    uint8_t bIn = Serial.read();
    checkXor ^= bIn;

    switch (currentState) {
        case SIZE:
            checkXor = bIn;

            if (bIn == 0) {
                break;
            }

            if (bIn & 0x80) {
                size  = ((int16_t) (bIn & 0x7F) << 8);
                currentState = SIZE2;
            }
            else {
                if (bIn > MAX_PACKET_SIZE) {
                    break;
                }

                size         = bIn;
                bufPos       = 0;
                currentState = CMD;
                break;
            }

        case SIZE2:
            size += bIn;
            if (bIn > MAX_PACKET_SIZE) {
                break;
            }

            size = bIn;
            currentState = CMD;
            break;

        case CMD:
            cmd = bIn;
            size--;

            if (size > 0) {
                currentState = DATA;
            }
            else {
                currentState = CRC;
            }
            break;

        case DATA:
            buf[bufPos++] = bIn;
            size--;

            if (size == 0) {
                currentState = CRC;
            }
            break;

        case CRC:
            if (checkXor == 0) {
                processPacket(cmd, buf, bufPos);
            }

            currentState = SIZE;
            break;
    }
}

void processCAN() {
    return;
}

void processPacket(uint8_t cmd, uint8_t* buf, uint16_t size) {
    switch (cmd) {
        case CMD_PING:
            Serial.println("ping !");
            break;

        case CMD_PONG:
            Serial.println("pong !");
            break;

        case CMD_SET_TUNINGS:
            Serial.println("tunings !");
            break;
    }
    return;
}
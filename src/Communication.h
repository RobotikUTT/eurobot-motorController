#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Wire.h>
#include <Arduino.h>
#include "I2cSlaveProtocol.h"


/**
 * @class Communication
 * @brief Communication class implemant the i2c slave protocol defined in class I2cSlaveProtocol. I2cSlaveProtocol doesnt' change between arduino boards but Communication will change as it contain command code.
 */
class Communication : public I2cSlaveProtocol
{
    public:

        /**
         * Called when a command is received
         * @param command - The command number
         * @param paramLength - The length of the param buffer
         * @param paramBuf - The list of bytes that constitue params
         */
        static void execute(byte command, byte paramLength, byte *paramBuf);
};

#endif
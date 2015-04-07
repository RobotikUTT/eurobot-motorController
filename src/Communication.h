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
         * Constructor with a data available line
         * @param address - i2c address
         * @param dataAvailablePin - Pin that will be put up when the slave has data available
         */
        static void open(byte address, byte dataAvailablePin);

        /**
         * Constructor without a data available line
         * @param address - i2c address
         */
        static void open(byte address);

        /**
         * Called when a command is received
         * @param command - The command number
         * @param paramLength - The length of the param buffer
         * @param paramBuf - The list of bytes that constitue params
         */
        static void execute(byte command, byte paramLength, byte *paramBuf);
        /**
         * Called when master request data
         */
        static void send();


        static byte lastCmd;
        static byte lastPingU8;
        static char lastPingS8;
        static unsigned short lastPingU16;
        static short lastPingS16;

        static const byte cmd_ping = 0x0;
};

#endif
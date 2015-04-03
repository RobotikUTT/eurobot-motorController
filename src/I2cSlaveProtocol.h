#ifndef I2CSLAVEPROTOCOL_H
#define I2CSLAVEPROTOCOL_H

#include <Wire.h>
#include <Arduino.h>


enum RcvState {
    CMD,
    LENGTH,
    PARAM,
    CHECK
};

/**
 * @class I2cSlaveProtocol
 * @brief I2cSlaveProtocol class that act as a slave over our communication protocol
 * @details Our protocol work over i2c.
 * i2c is a master/slave(s) protocol, all slaves with i2c will never do something if the master doesn't ask him. 
 * 
 * In our protocol, we have a facultative wire called `data available` between master and each slave, so the slave can say when he 
 * have something to say (LOW is when there is nothing to say)
 * 
 * The Master use this type of packets
 * * 8 bits : Command ID
 * * 8 bits : Param length
 * * X bits : Params
 * * 2 bits : Rotating packet number. (On each packet send to the same slave, the number will be rotated by master) |
 * * 6 bits : 8 bit XOR checksum modulo 2^6                                                                         | => Check byte
 *  
 * All slaves uses this type of packets
 * * 8 bits : Last command received (Check byte)
 * * 8 bits : Command ID
 * * 8 bits : Param length
 * * X bits : Params
 * * 6 bits : 8 bit XOR checksum
 *
 * If slave has nothing to say, it can only send the last command checkbyte.
 * In this way if you want to see if the command has been executed, you just have to read the first byte and check
 * if the value is the same as your command checkbyte
 */
class I2cSlaveProtocol
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
         * Called when data is received
         * @param count - Byte count available in receive buffer
         */
        static void received(int count);

        /**
         * Called when data is required
         */
        static void requested();

        /**
         * Called when a command is received
         * @param command - The command number
         * @param paramLength - The length of the param buffer
         * @param paramBuf - The list of bytes that constitue params
         */
        static void execute(byte command, byte paramLength, byte *paramBuf);


// ----------------------------------------------
//      Extract functions
// ----------------------------------------------

        /**
         * Extract an unsigned int of 8 bit from a buffer
         * @param pos - A pointer to the pos value (that will be decremented of the size extracted)
         * @param paramBuf - The byte array
         */
        static byte extractUInt8(byte *pos, byte *buf);

        /**
         * Extract an int of 8 bit from a buffer
         * @param pos - A pointer to the pos value (that will be decremented of the size extracted)
         * @param paramBuf - The byte array
         */
        static char extractInt8(byte *pos, byte *buf);

        /**
         * Extract an unsigned int of 16 bit from a buffer
         * @param pos - A pointer to the pos value (that will be decremented of the size extracted)
         * @param paramBuf - The byte array
         */
        static short extractUInt16(byte *pos, byte *buf);

        /**
         * Extract an int of 16 bit from a buffer
         * @param pos - A pointer to the pos value (that will be decremented of the size extracted)
         * @param paramBuf - The byte array
         */
        static unsigned short extractInt16(byte *pos, byte *buf);




        static byte dataAvailablePin;
        static byte lastRcvCheck;
        static RcvState rcvState;
        static byte rcvCmd;
        static byte rcvLength;
        static byte rcvPos;
        static byte rcvBuf[32];
        static byte rcvCheck;

};

#endif
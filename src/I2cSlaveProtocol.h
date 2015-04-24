#ifndef I2CSLAVEPROTOCOL_H
#define I2CSLAVEPROTOCOL_H

#include <Wire.h>
#include <Arduino.h>

#define I2CSP_SND_BUF 32
#define I2CSP_RCV_BUF 32

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
 * * 8 bits : Param length
 * * X bits : Params
 * * 6 bits : 8 bit XOR checksum
 *
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
         * @param dataAvailableCmd - The cmd number that will make the pin fall after a call of dataAvailable()
         */
        static void open(byte address, char dataAvailablePin, char dataAvailableCmd);

        /**
         * Constructor without a data available line
         * @param address - i2c address
         */
        static void open(byte address);



        /**
         * Called when data is required
         */
        static void requested();

        /**
         * Called when data is requested from master
         */
        static void sendDefault();

        /**
         * Set the function that will be executed when data is requested (by default this function is `sendDefault()`)
         * @param a pointer to the function that will be executed wich should have the same structure as `sendDefault()`
         */
        static void setSend(void (*send)());



        /**
         * Called when data is received
         * @param count - Byte count available in receive buffer
         */
        static void received(int count);      
        
        /**
         * Set the function that will be executed when a command is received (by default this function is `executeDefault()`)
         * @param a pointer to the function that will be executed wich should have the same structure as `executeDefault()`
         */
        static void setExecute(void (*execute)(byte command, byte paramLength, byte *paramBuf));

        /**
         * Called when a command is received
         * @param command - The command number
         * @param paramLength - The length of the param buffer
         * @param paramBuf - The list of bytes that constitue params
         */
        static void executeDefault(byte command, byte paramLength, byte *paramBuf);


        /**
         * Will rise the data available pin
         */
        static void dataAvailable();

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
        static unsigned short extractUInt16(byte *pos, byte *buf);

        /**
         * Extract an int of 16 bit from a buffer
         * @param pos - A pointer to the pos value (that will be decremented of the size extracted)
         * @param paramBuf - The byte array
         */
        static short extractInt16(byte *pos, byte *buf);

        /**
         * Extract an float of 32 bit from a buffer
         * @param pos - A pointer to the pos value (that will be decremented of the size extracted)
         * @param paramBuf - The byte array
         */
        static float extractFloat(byte *pos, byte *buf);

// ----------------------------------------------
//      Send functions
// ----------------------------------------------

        /**
        * @brief Add a byte as unsigned integer to parameters
        * @param data - The value that will be added to the buffer
        */
        static void addUInt8(byte data);

        /**
        * @brief Add a byte as integer to parameters
        * @param data - The value that will be added to the buffer
        */
        static void addInt8(char data);

        /**
        * @brief Add two bytes as unsigned integer to parameters
        * @param data - The value that will be added to the buffer
        */
        static void addUInt16(unsigned short data);

        /**
        * @brief Add two bytes as integer to parameters
        * @param data - The value that will be added to the buffer
        */
        static void addInt16(short data);

        /**
        * @brief Add four bytes as float to parameters
        * @param data - The value that will be added to the buffer
        */
        static void addFloat(float data);


        static void (*execute)(byte command, byte paramLength, byte *paramBuf);
        static void (*send)();

        static char dataAvailablePin;
        static char dataAvailableCmd;
        static byte lastRcvCheck;

        static RcvState rcvState;
        static byte rcvCmd;
        static byte rcvLength;
        static byte rcvPos;
        static byte rcvBuf[I2CSP_RCV_BUF];
        static byte rcvCheck;

        static byte sndPos;
        static byte sndBuf[I2CSP_SND_BUF];

};

#endif
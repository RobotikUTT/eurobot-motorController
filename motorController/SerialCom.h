#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <Arduino.h> 

#define SERIALCOM_SERIAL Serial
#define SERIALCOM_SENDBUFFER 16 //Send buffer size in byte

typedef signed char sbyte;

class SerialCom
{
    /**
    * @Class SerialCom
    * @brief Let you send and receive command over serial port
    *
    * Let you send and receive command over serial port
    *
    * Protocol definition :
    * * ffffff : 3 constants bytes that show the beginning of a command
    * * 3 bits : Priority : 111 if the whole command is repeated 3 times or 000 if one time.
    * * 5 bits : The command ID
    * * ------ : Data
    * * 1 byte : XOR sum from priotity to end of data
    * 
    * Priority is a boolean that tell the receiver to ignore the 3 next command with the 
    * same XOR sum or use the next command if the first XOR sum is false.
    * 9600 bauds
    */


    public:
    
        static const sbyte command_debug = 0;

        /**
        * @brief Construct and empty SerialCom object
        */
        SerialCom();

        /**
        * @brief Destructor
        */
        ~SerialCom();

        /**
        * @brief Clear the send buffer and the command id
        */
        void clearSendBuffer();

        /**
        * @brief Set the command that will be send
        *
        * @param commandID  The command id. Please use static constants command_*.
        */
        void setSendCommand(sbyte commandID);

        /**
        * @brief Add byte to the send buffer
        *
        * @param data   The byte (unsigned) added to the buffer
        */
        void writeByte(byte data);

        /**
        * @brief Send the command to the serial port and clear the send buffer
        */
        void send();

    private:
        /**
        * @brief Command ID that will be send when send() will be used
        */
        sbyte sendCommandID;

        /**
        * @brief Send buffer
        */
        byte sendBuffer[SERIALCOM_SENDBUFFER];

        /**
        * @brief Next write position in send buffer
        */
        byte sendPos;


};

#endif
#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <Arduino.h>
#include "SerialComCmd.h"

#define SERIALCOM_EXTRACT_INT8(pos, buffer)    (SERIALCOM_INT8)(buffer[pos]);
#define SERIALCOM_EXTRACT_UINT8(pos, buffer)    (SERIALCOM_UINT8)(buffer[pos]);
#define SERIALCOM_EXTRACT_INT16(pos, buffer)    (SERIALCOM_INT16)((buffer[pos]<<8) | buffer[pos+1]);
#define SERIALCOM_EXTRACT_UINT16(pos, buffer)    (SERIALCOM_UINT16)((buffer[pos]<<8) | buffer[pos+1]);
#define SERIALCOM_ERROR_NOTCMD 255

//Interface and buffer configuration
#define SERIALCOM_SERIAL Serial
#define SERIALCOM_SENDBUFFER 16 //Send buffer size in byte
#define SERIALCOM_RECEIVEBUFFER 3*SERIALCOM_SENDBUFFER
#define SERIALCOM_READARGBUFFER SERIALCOM_SENDBUFFER

//Type configuration
#define SERIALCOM_INT8 char
#define SERIALCOM_UINT8 unsigned char
#define SERIALCOM_INT16 short
#define SERIALCOM_UINT16 unsigned short

class SerialCom
{
    /**
    * @Class SerialCom
    * @brief Let you send and receive command over serial port
    *
    * Let you send and receive command over serial port.
    *
    * Protocol definition :
    * * ffffff : 3 constants bytes that show the beginning of a command
    * * 1 byte : The command ID
    * * ------ : Data
    * * 1 byte : XOR sum from priotity to end of data
    * 9600 bauds
    */


    public:

        /**
        * @brief Construct and empty SerialCom object
        */
        SerialCom();


        // ----------------------------------------------
        //      Send functions
        // ----------------------------------------------

        /**
        * @brief Clear the send buffer and the command id
        */
        void clearSendBuffer();

        /**
        * @brief Set the command that will be send
        *
        * @param commandID  The command id
        */
        void setSendCommand(byte commandID);

        /**
        * @brief Send the command to the serial port and clear the send buffer
        */
        void send();



        /**
        * @brief Add a byte as integer to parameters
        *
        * @param data  The value that will be added to the buffer
        */
        void writeInt8(SERIALCOM_INT8 data);

        /**
        * @brief Add a byte as unsigned integer to parameters
        *
        * @param data  The value that will be added to the buffer
        */
        void writeUInt8(SERIALCOM_UINT8 data);

        /**
        * @brief Add two bytes as integer to parameters
        *
        * @param data  The value that will be added to the buffer
        */
        void writeInt16(SERIALCOM_INT16 data);

        /**
        * @brief Add two bytes as unsigned integer to parameters
        *
        * @param data  The value that will be added to the buffer
        */
        void writeUInt16(SERIALCOM_UINT16 data);



// ----------------------------------------------
//      Reception functions
// ----------------------------------------------

        /**
        * @brief Check if there is new packets and execute them.
        * 
        * Check if there is new packets and execute one of them. 
        * You should put this function in your main loop.
        */
        void doReadJob();

    private:
        /**
        * @brief Command ID that will be send when send() will be used
        */
        byte sendCommandID;

        /**
        * @brief Buffer that contain the sending packet during creation and before sending it
        */
        byte sendBuffer[SERIALCOM_SENDBUFFER];

        /**
        * @brief Next write position in send buffer
        */
        byte sendPos;

        /**
        * @brief Buffer that contain new received bytes before read them
        */
        byte receiveBuffer[SERIALCOM_RECEIVEBUFFER];

        /**
        * @brief Next reading position in receive buffer
        */
        byte receiveBufferReadPos;
        /**
        * @brief Next writing position in receive buffer
        */
        byte receiveBufferWritePos;

        /**
        * @brief Current state of the reading process of a packet
        */
        byte readState;
        /**
        * @brief Progress (in bytes) of the current state of the reading process of a packet
        */
        byte readStateProgress;

        /**
        * @brief Length in bytes of the argument part of the command
        */
        byte readArgLength;

        /**
        * @brief ID of the command received
        */
        byte readCmd;

        /**
        * @brief Xor that is calculated during reception
        */
        byte readXor;

        /**
        * @brief Buffer that contain all bytes of arguments during reception and before execution
        */
        byte readArgBuffer[SERIALCOM_READARGBUFFER];

        /**
        * @brief Reset all vars to find a new packet because the current one is over or not valid
        */
        void clearReadState();

        /**
        * @brief State that is assigned to readState to indicate that we are waiting for a packet begining constant
        */
        static const byte STATE_SIGN = 0;

        /**
        * @brief State that is assigned to readState to indicate that we are waiting for the command ID
        */
        static const byte STATE_COMMAND = 1;

        /**
        * @brief State that is assigned to readState to indicate that we are waiting for arguments
        */
        static const byte STATE_ARGUMENTS = 2;

        /**
        * @brief State that is assigned to readState to indicate that we are waiting for a byte that contain the xor sum
        */
        static const byte STATE_XOR = 3;

};

#endif

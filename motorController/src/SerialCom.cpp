#include "SerialCom.h"

SerialCom::SerialCom()
{
    //Init serial
    SERIALCOM_SERIAL.begin(9600);

    //init reception vars
    clearReadState();
    this->receiveBufferWritePos = 0;
    this->receiveBufferReadPos = 0;

    //Init or clear send vars
    clearSendBuffer();
}



// ----------------------------------------------
//      Send functions
// ----------------------------------------------

void SerialCom::clearSendBuffer()
{
    this->sendCommandID = 0;
    this->sendPos = 0;
}

void SerialCom::setSendCommand(byte commandID)
{
    this->sendCommandID = commandID;
}

void SerialCom::send()
{
    byte xorSum;
    byte* buffer;
    buffer = (byte*)malloc(5 + this->sendPos);

    //Add constant
    buffer[0] = 0xff;
    buffer[1] = 0xff;
    buffer[2] = 0xff;

    //Add command ID
    buffer[3] = this->sendCommandID;

    //Add data and calculate xor
    xorSum = this->sendCommandID;
    for (byte i = 0; i < this->sendPos; ++i)
    {
        buffer[4 + i] = this->sendBuffer[i];
        xorSum ^= this->sendBuffer[i];
    }
    buffer[this->sendPos + 4] = xorSum;

    //Send
    SERIALCOM_SERIAL.write(buffer, 5 + this->sendPos);

    free(buffer);

    clearSendBuffer();
}


void SerialCom::writeInt8(SERIALCOM_INT8 data)
{
    SerialCom::writeUInt8((SERIALCOM_UINT8)data);
}
void SerialCom::writeUInt8(SERIALCOM_UINT8 data)
{
    if(this->sendPos < SERIALCOM_SENDBUFFER)
    {
        this->sendBuffer[this->sendPos++] = data;
    }
}
void SerialCom::writeInt16(SERIALCOM_INT16 data)
{
    this->writeUInt16((SERIALCOM_UINT16) data);
}
void SerialCom::writeUInt16(SERIALCOM_UINT16 data)
{
    if(this->sendPos + 1 < SERIALCOM_SENDBUFFER)
    {
        this->sendBuffer[this->sendPos++] = (data >> 8)&8;
        this->sendBuffer[this->sendPos++] = data&8;
    }
}




// ----------------------------------------------
//      Reception functions
// ----------------------------------------------

void SerialCom::clearReadState()
{
    this->readState = STATE_SIGN;
    this->readStateProgress = 0;
    this->readCmd = 0;
    this->readArgLength = 0;
}

void SerialCom::doReadJob()
{
    //Add new bytes to the receive buffer
    while (SERIALCOM_SERIAL.available() > 0)
    {
        this->receiveBuffer[this->receiveBufferWritePos] = Serial.read();
        this->receiveBufferWritePos++;
        if(this->receiveBufferWritePos >= SERIALCOM_RECEIVEBUFFER)
        {
            this->receiveBufferWritePos = 0;
        }
    }

    //Read new bytes until end of packets
    bool stop = false;
    while(this->receiveBufferWritePos != this->receiveBufferReadPos && !stop)
    {
        switch(this->readState)
        {
            case STATE_SIGN :
                if(this->receiveBuffer[this->receiveBufferReadPos] == 0xff)
                {
                    this->readStateProgress++;
                    if(this->readStateProgress >= 3)
                    {
                        //going to the next state
                        this->readState = STATE_COMMAND;
                        this->readStateProgress = 0;
                    }
                }
                else
                {
                    clearReadState();
                }
                break;
            case STATE_COMMAND :
                this->readCmd = this->receiveBuffer[this->receiveBufferReadPos];
                this->readArgLength = SerialComCmd::getCommandLength(this->readCmd);
                this->readXor = this->readCmd;
                if(this->readArgLength == SERIALCOM_ERROR_NOTCMD || SERIALCOM_READARGBUFFER < this->readArgLength)
                {
                    //Error : command doen't exist or param length is too long
                    clearReadState();
                }
                else
                {
                    //going to the next state
                    this->readState = STATE_ARGUMENTS;
                    this->readStateProgress = 0;
                }
                break;
            case STATE_ARGUMENTS :
                this->readArgBuffer[this->readStateProgress] = this->receiveBuffer[this->receiveBufferReadPos];
                this->readXor ^= this->receiveBuffer[this->receiveBufferReadPos];
                this->readStateProgress++;
                if(this->readStateProgress >= this->readArgLength)
                {
                    //going to the next state
                    this->readState = STATE_XOR;
                    this->readStateProgress = 0;
                }
                break;
            case STATE_XOR :
                if(this->readXor == this->receiveBuffer[this->receiveBufferReadPos])
                {
                    SerialComCmd::executeCommand(this->readCmd, this->readArgBuffer);

                    //Force to stop after one command executed to be sure the serial communication doens't block the rest of the program
                    stop = true;
                }
                clearReadState();
        }

        //Increment read position
        this->receiveBufferReadPos++;
        if(this->receiveBufferReadPos >= SERIALCOM_RECEIVEBUFFER)
        {
            this->receiveBufferReadPos = 0;
        }

    }


}

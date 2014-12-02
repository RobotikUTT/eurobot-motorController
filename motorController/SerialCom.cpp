#include "SerialCom.h";


SerialCom::SerialCom()
{
    SERIALCOM_SERIAL.begin(9600);

    //Init send vars
    clearSendBuffer();
}

SerialCom::~SerialCom()
{}

void SerialCom::clearSendBuffer()
{
    this->sendCommandID = -1;
    this->sendPos = 0;
}

void SerialCom::setSendCommand(sbyte commandID)
{
    if(commandID >= 0 && commandID <= 0x1f)
        this->sendCommandID = commandID;
}

void SerialCom::setSendPriority(bool priority)
{
    this->sendPriority = priority;
}

void SerialCom::writeByte(byte data)
{
    if(this->sendPos < SERIALCOM_SENDBUFFER)
    {
        this->sendBuffer[this->sendPos] = data;
    }
}

void SerialCom::send()
{
    if(sendCommandID >= 0)
    {        
        byte tmp;
        byte* buffer;
        buffer = (byte*)malloc(5 + this->sendPos);

        //Add constant
        buffer[0] = 0xff;
        buffer[1] = 0xff;
        buffer[2] = 0xff;

        //Add Priority and command ID
        tmp = 0;
        if(this->sendPriority)
            tmp = 0b1110000;
        tmp |= this->sendCommandID;
        buffer[3] = tmp;

        //Add data and calculate xor
        for (byte i = 0; i < this->sendPos; ++i)
        {
            buffer[4 + i] = this->sendBuffer[i];
            tmp ^= this->sendBuffer[i];
        }
        buffer[this->sendPos + 4] = tmp;

        //Send
        SERIALCOM_SERIAL.write(buffer, 5 + this->sendPos);
        if (this->sendPriority)
        {
            SERIALCOM_SERIAL.write(buffer, 5 + this->sendPos);
            SERIALCOM_SERIAL.write(buffer, 5 + this->sendPos);
        }
    }

    clearSendBuffer();
}
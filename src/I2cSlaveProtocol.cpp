#include "I2cSlaveProtocol.h"
        
byte I2cSlaveProtocol::address = 0;
char I2cSlaveProtocol::dataAvailablePin = -1;
char I2cSlaveProtocol::dataAvailableCmd = -1;
byte I2cSlaveProtocol::lastRcvCheck = 0xff;

RcvState I2cSlaveProtocol::rcvState = CMD;
byte I2cSlaveProtocol::rcvCmd = 0;
byte I2cSlaveProtocol::rcvLength = 0;
byte I2cSlaveProtocol::rcvPos = 0;
byte I2cSlaveProtocol::rcvBuf[I2CSP_RCV_BUF];
byte I2cSlaveProtocol::rcvCheck = 0xff;
byte I2cSlaveProtocol::sndPos = 0;
byte I2cSlaveProtocol::sndBuf[I2CSP_SND_BUF];

void (*I2cSlaveProtocol::execute)(byte command, byte paramLength, byte *paramBuf) = I2cSlaveProtocol::executeDefault;
void (*I2cSlaveProtocol::send)() = I2cSlaveProtocol::sendDefault;


void I2cSlaveProtocol::open(byte address, char dataAvailablePin, char dataAvailableCmd)
{
    I2cSlaveProtocol::dataAvailableCmd = dataAvailableCmd;
    I2cSlaveProtocol::dataAvailablePin = dataAvailablePin;
    pinMode(dataAvailablePin, OUTPUT);
    I2cSlaveProtocol::open(address);
}

void I2cSlaveProtocol::open(byte address)
{
    Wire.begin(address);
    Wire.onReceive(I2cSlaveProtocol::received);
    Wire.onRequest(I2cSlaveProtocol::requested);
    Serial.println("Com i2c Ready !");
}

void I2cSlaveProtocol::requested()

{
    //reserve the two first bytes for header
    I2cSlaveProtocol::sndPos = 2;

    I2cSlaveProtocol::send();

    //Write header
    I2cSlaveProtocol::sndBuf[0] = I2cSlaveProtocol::lastRcvCheck;
    I2cSlaveProtocol::sndBuf[1] = I2cSlaveProtocol::sndPos - 2;

    // Checksum calculation
    byte chk = I2cSlaveProtocol::sndPos - 2;
    for (int i = 2; i < sndPos; ++i)
    {
        chk ^= I2cSlaveProtocol::sndBuf[i];
    }
    I2cSlaveProtocol::addUInt8(chk);

    // Debug
    Serial.print("SndBuf:");
    for (int i = 0; i < I2cSlaveProtocol::sndPos; ++i)
    {
        Serial.print(I2cSlaveProtocol::sndBuf[i],HEX);
        Serial.print("|");
    }
    Serial.println();

    //Send via i2c
    Wire.write(I2cSlaveProtocol::sndBuf, I2cSlaveProtocol::sndPos);
}

void I2cSlaveProtocol::received(int count)
{

    // Debug
    Serial.print("RcvBuf:");

    while(Wire.available())
    {
        byte curByte = Wire.read();

        // Debug
        Serial.print(curByte,HEX);
        Serial.print("|");

        switch(I2cSlaveProtocol::rcvState)
        {
            case CMD:
            {
                I2cSlaveProtocol::rcvCmd = curByte;
                I2cSlaveProtocol::rcvCheck = I2cSlaveProtocol::rcvCmd;
                I2cSlaveProtocol::rcvState = LENGTH;
                break;
            }
            case LENGTH:
            {
                I2cSlaveProtocol::rcvLength = curByte;
                I2cSlaveProtocol::rcvCheck ^= I2cSlaveProtocol::rcvLength;
                I2cSlaveProtocol::rcvState = PARAM;
                I2cSlaveProtocol::rcvPos = 0;
                if(I2cSlaveProtocol::rcvLength == 0)
                    I2cSlaveProtocol::rcvState = CHECK;
                break;
            }
            case PARAM:
            {
                I2cSlaveProtocol::rcvBuf[I2cSlaveProtocol::rcvPos] = curByte;
                I2cSlaveProtocol::rcvCheck ^= I2cSlaveProtocol::rcvBuf[I2cSlaveProtocol::rcvPos];
                I2cSlaveProtocol::rcvPos++;

                if(I2cSlaveProtocol::rcvPos >= I2CSP_RCV_BUF)
                {
                    Serial.println();
                    Serial.println("Warn : Packet dropped (buffer overflow)");
                    I2cSlaveProtocol::rcvState = CMD;
                }
                else if(I2cSlaveProtocol::rcvPos >= I2cSlaveProtocol::rcvLength)
                {
                    I2cSlaveProtocol::rcvState = CHECK;
                }
                break;
            }
            case CHECK:
            {
                byte readCheck = curByte;
                //Check sum that is on the last 6 bits of the command
                if((readCheck&0x3f) == (I2cSlaveProtocol::rcvCheck%64))
                {
                    // Debug
                    Serial.println();

                    //Make dataAvailablePin falling if this is the right command
                    if (I2cSlaveProtocol::dataAvailableCmd == I2cSlaveProtocol::rcvCmd)
                        digitalWrite(I2cSlaveProtocol::dataAvailablePin, LOW);

                    //execute command
                    I2cSlaveProtocol::execute(I2cSlaveProtocol::rcvCmd, I2cSlaveProtocol::rcvLength, I2cSlaveProtocol::rcvBuf);
                    I2cSlaveProtocol::lastRcvCheck = I2cSlaveProtocol::rcvCheck;
                }
                else
                {
                    Serial.println();
                    Serial.println("Warn : Packet dropped (bad checksum)");
                }
                I2cSlaveProtocol::rcvState = CMD;
                break;
            }
        }
    }
    // Debug
    Serial.println();

    //Reset state if packet not received totally
    if(I2cSlaveProtocol::rcvState != CMD)
    {
        Serial.println("Warn : Packet dropped (not enough bytes)");
        I2cSlaveProtocol::rcvState = CMD;
    }
}


void I2cSlaveProtocol::sendDefault()
{
    Serial.println("Warn : There is nothing to request");
}

void I2cSlaveProtocol::setSend(void (*send)())
{
    I2cSlaveProtocol::send = send;
}

void I2cSlaveProtocol::executeDefault(byte command, byte length, byte* params)
{
    Serial.print("Warn : Command 0x");
    Serial.print(command, HEX);
    Serial.println(" doen't exist (There is no functions defined)");
}

void I2cSlaveProtocol::setExecute(void (*execute)(byte command, byte paramLength, byte *paramBuf))
{
    I2cSlaveProtocol::execute = execute;
}

void I2cSlaveProtocol::dataAvailable()
{
    if(I2cSlaveProtocol::dataAvailablePin != -1)
    {
        digitalWrite(I2cSlaveProtocol::dataAvailablePin, HIGH);
    }
}

void I2cSlaveProtocol::addUInt8(byte data)
{
    if(I2cSlaveProtocol::sndPos < I2CSP_SND_BUF)
    {
        I2cSlaveProtocol::sndBuf[I2cSlaveProtocol::sndPos++] = data&0xff;
    }
    else
        Serial.print("Warn : Send buffer overflow");
}

void I2cSlaveProtocol::addInt8(char data)
{
     I2cSlaveProtocol::addUInt8((byte) data);
}
void I2cSlaveProtocol::addUInt16(unsigned short data)
{
    if(I2cSlaveProtocol::sndPos + 1 < I2CSP_SND_BUF)
    {
        I2cSlaveProtocol::sndBuf[I2cSlaveProtocol::sndPos++] = (data >> 8)&0xff;
        I2cSlaveProtocol::sndBuf[I2cSlaveProtocol::sndPos++] = data&0xff;
    }
    else
        Serial.print("Warn : Send buffer overflow");
}
void I2cSlaveProtocol::addInt16(short data)
{
    addUInt16((unsigned short) data);
}

void I2cSlaveProtocol::addFloat(float data)
{
    byte*  bytes = (byte*) &data;
    I2cSlaveProtocol::sndBuf[I2cSlaveProtocol::sndPos++] = bytes[3];
    I2cSlaveProtocol::sndBuf[I2cSlaveProtocol::sndPos++] = bytes[2];
    I2cSlaveProtocol::sndBuf[I2cSlaveProtocol::sndPos++] = bytes[1];
    I2cSlaveProtocol::sndBuf[I2cSlaveProtocol::sndPos++] = bytes[0];
}
void I2cSlaveProtocol::addInt32(long data)
{
    byte*  bytes = (byte*) &data;
    I2cSlaveProtocol::sndBuf[I2cSlaveProtocol::sndPos++] = bytes[3];
    I2cSlaveProtocol::sndBuf[I2cSlaveProtocol::sndPos++] = bytes[2];
    I2cSlaveProtocol::sndBuf[I2cSlaveProtocol::sndPos++] = bytes[1];
    I2cSlaveProtocol::sndBuf[I2cSlaveProtocol::sndPos++] = bytes[0];
}


byte I2cSlaveProtocol::extractUInt8(byte *pos, byte *buf)
{
    (*pos)++;
    return buf[*pos-1];
}

char I2cSlaveProtocol::extractInt8(byte *pos, byte *buf)
{
    (*pos)++;
    return (char) buf[*pos-1];
}

unsigned short I2cSlaveProtocol::extractUInt16(byte *pos, byte *buf)
{
    (*pos) += 2;
    return (unsigned short) ((buf[*pos-2]<<8) | buf[*pos-1]);
}

short I2cSlaveProtocol::extractInt16(byte *pos, byte *buf)
{
    (*pos) += 2;
    return (short) ((buf[*pos-2]<<8) | buf[*pos-1]);
}

float I2cSlaveProtocol::extractFloat(byte *pos, byte *buf)
{
    (*pos) += 4;

    float rtn = 0;
    byte*  bytes = (byte*) &rtn;

    bytes[0] = buf[*pos-1];
    bytes[1] = buf[*pos-2];
    bytes[2] = buf[*pos-3];
    bytes[3] = buf[*pos-4];

    return rtn ;
}
long I2cSlaveProtocol::extractInt32(byte *pos, byte *buf)
{
    (*pos) += 4;

    long rtn = 0;
    byte*  bytes = (byte*) &rtn;

    bytes[0] = buf[*pos-1];
    bytes[1] = buf[*pos-2];
    bytes[2] = buf[*pos-3];
    bytes[3] = buf[*pos-4];

    return rtn ;
}
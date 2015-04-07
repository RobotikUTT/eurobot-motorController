#include "I2cSlaveProtocol.h"
        
byte I2cSlaveProtocol::dataAvailablePin = 0xff;
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


void I2cSlaveProtocol::open(byte address, byte dataAvailablePin)
{
    I2cSlaveProtocol::dataAvailablePin = dataAvailablePin;
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
    // Serial.print("SndBuf:");
    // for (int i = 0; i < I2cSlaveProtocol::sndPos; ++i)
    // {
    //     Serial.print(I2cSlaveProtocol::sndBuf[i],HEX);
    //     Serial.print("|");
    // }
    // Serial.println();

    //Send via i2c
    Wire.write(I2cSlaveProtocol::sndBuf, I2cSlaveProtocol::sndPos);
}

void I2cSlaveProtocol::received(int count)
{

    // Debug
    // Serial.print("RcvBuf:");
    // for (int i = 0; i < 6; ++i)
    // {
    //     Serial.print(params[i],HEX);
    //     Serial.print("|");
    // }
    // Serial.println();

    while(Wire.available())
    {
        switch(I2cSlaveProtocol::rcvState)
        {
            case CMD:
            {
                I2cSlaveProtocol::rcvCmd = Wire.read();
                I2cSlaveProtocol::rcvCheck = I2cSlaveProtocol::rcvCmd;
                I2cSlaveProtocol::rcvState = LENGTH;
                break;
            }
            case LENGTH:
            {
                I2cSlaveProtocol::rcvLength = Wire.read();
                I2cSlaveProtocol::rcvCheck ^= I2cSlaveProtocol::rcvLength;
                I2cSlaveProtocol::rcvState = PARAM;
                I2cSlaveProtocol::rcvPos = 0;
                break;
            }
            case PARAM:
            {
                I2cSlaveProtocol::rcvBuf[I2cSlaveProtocol::rcvPos] = Wire.read();
                I2cSlaveProtocol::rcvCheck ^= I2cSlaveProtocol::rcvBuf[I2cSlaveProtocol::rcvPos];
                I2cSlaveProtocol::rcvPos++;

                if(I2cSlaveProtocol::rcvPos >= I2CSP_RCV_BUF)
                {
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
                byte readCheck = Wire.read();
                //Check sum that is on the last 6 bits of the command
                if((readCheck&0x3f) == (I2cSlaveProtocol::rcvCheck%64))
                {
                    I2cSlaveProtocol::execute(I2cSlaveProtocol::rcvCmd, I2cSlaveProtocol::rcvLength, I2cSlaveProtocol::rcvBuf);
                    I2cSlaveProtocol::lastRcvCheck = I2cSlaveProtocol::rcvCheck;
                }
                else
                {
                    Serial.println("Warn : Packet dropped (bad checksum)");
                }
                I2cSlaveProtocol::rcvState = CMD;
                break;
            }
        }
    }

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
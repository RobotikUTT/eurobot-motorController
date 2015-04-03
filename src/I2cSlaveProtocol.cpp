#include "I2cSlaveProtocol.h"
        
byte I2cSlaveProtocol::dataAvailablePin = 0xff;
byte I2cSlaveProtocol::lastRcvCheck = 0xff;
RcvState I2cSlaveProtocol::rcvState = CMD;
byte I2cSlaveProtocol::rcvCmd = 0;
byte I2cSlaveProtocol::rcvLength = 0;
byte I2cSlaveProtocol::rcvPos = 0;
byte I2cSlaveProtocol::rcvBuf[32];
byte I2cSlaveProtocol::rcvCheck = 0xff;

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
    Serial.println("Com Ready !");
}

void I2cSlaveProtocol::requested()
{
    Wire.write(I2cSlaveProtocol::rcvCheck);
    Serial.print("Warn : There is nothing to require");
}

void I2cSlaveProtocol::received(int count)
{
    Serial.print("Count:");
    Serial.println(count);

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
                if(I2cSlaveProtocol::rcvPos >= I2cSlaveProtocol::rcvLength || I2cSlaveProtocol::rcvPos >= 32)
                    I2cSlaveProtocol::rcvState = CHECK;
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
                I2cSlaveProtocol::rcvState = CMD;
                break;
            }
            default:
                I2cSlaveProtocol::rcvState = CMD;
        }
    }

    //Reset state if packet not received totally
    I2cSlaveProtocol::rcvState = CMD;
}


void I2cSlaveProtocol::execute(byte command, byte length, byte* params)
{
    Serial.print("Warn : Command 0x");
    Serial.print(command, HEX);
    Serial.println(" doen't exist (There is no functions defined)");
}
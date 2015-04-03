#include "Communication.h"
        
byte Communication::dataAvailablePin = 0xff;
byte Communication::lastRcvCheck = 0xff;


RcvState Communication::rcvState = CMD;
byte Communication::rcvCmd = 0;
byte Communication::rcvLength = 0;
byte Communication::rcvPos = 0;
byte Communication::rcvBuf[32];
byte Communication::rcvCheck = 0xff;

void Communication::open(byte address, byte dataAvailablePin)
{
    Communication::dataAvailablePin = dataAvailablePin;
    Communication::open(address);
}

void Communication::open(byte address)
{
    Wire.begin(address);
    Wire.onReceive(Communication::received);
    Serial.println("Com Ready !");
}

void Communication::received(int count)
{
    Serial.print("Count:");
    Serial.println(count);

    while(Wire.available())
    {
        switch(Communication::rcvState)
        {
            case CMD:
            {
                Communication::rcvCmd = Wire.read();
                Communication::rcvCheck = Communication::rcvCmd;
                Communication::rcvState = LENGTH;
                break;
            }
            case LENGTH:
            {
                Communication::rcvLength = Wire.read();
                Communication::rcvCheck ^= Communication::rcvLength;
                Communication::rcvState = PARAM;
                Communication::rcvPos = 0;
                break;
            }
            case PARAM:
            {
                    Communication::rcvBuf[Communication::rcvPos] = Wire.read();
                    Communication::rcvCheck ^= Communication::rcvBuf[Communication::rcvPos];
                    Communication::rcvPos++;
                    if(Communication::rcvPos >= Communication::rcvLength || Communication::rcvPos >= 32)
                        Communication::rcvState = CHECK;
                break;
            }
            case CHECK:
            {
                byte readCheck = Wire.read();
                //Check sum that is on the last 6 bits of the command
                if((readCheck&0x3f) == (Communication::rcvCheck%64))
                    Communication::execute(Communication::rcvCmd, Communication::rcvLength, Communication::rcvBuf);
                Communication::rcvState = CMD;
                break;
            }
            default:
                Communication::rcvState = CMD;
        }
    }

    //Reset state if packet not received totally
    Communication::rcvState = CMD;
}


void Communication::execute(byte command, byte length, byte* params)
{
    switch(command)
    {

    }
}
#include "Communication.h"


byte Communication::lastCmd = 0xff;
byte Communication::lastPingU8 = 0;
char Communication::lastPingS8 = 0;
unsigned short Communication::lastPingU16 = 0;
short Communication::lastPingS16 = 0;


void Communication::open(byte address, byte dataAvailablePin)
{
    Communication::dataAvailablePin = dataAvailablePin;
    Communication::open(address);
}

void Communication::open(byte address)
{
    Communication::setExecute(&Communication::execute);
    Communication::setSend(&Communication::send);

    I2cSlaveProtocol::open(address);
}


void Communication::execute(byte command, byte length, byte* params)
{
    Communication::lastCmd = command;
    switch(command)
    {
        case Communication::cmd_ping:
        {
            if(length >= 6)
            {

                byte pos = 0;
                Communication::lastPingU8 = extractUInt8(&pos, params);
                Communication::lastPingS8 = extractInt8(&pos, params);
                Communication::lastPingU16 = extractUInt16(&pos, params);
                Communication::lastPingS16 = extractInt16(&pos, params);

                // Debug
                // Serial.print("Ping recu :");
                // Serial.print(Communication::lastPingU8, DEC);
                // Serial.print(" : ");
                // Serial.print(Communication::lastPingS8, DEC);
                // Serial.print(" : ");
                // Serial.print(Communication::lastPingU16, DEC);
                // Serial.print(" : ");
                // Serial.println(Communication::lastPingS16, DEC);
            }
            break;
        }
        default:
        {
            Serial.print("Warn : Command 0x");
            Serial.print(command, HEX);
            Serial.println(" doen't exist");
        }
    }
}

void Communication::send()
{
    switch(Communication::lastCmd)
    {
        case Communication::cmd_ping:
        {
            // Serial.println("Send ping");
            Communication::addUInt8(Communication::lastPingU8 + 1);
            Communication::addInt8(Communication::lastPingS8 + 1);
            Communication::addUInt16(Communication::lastPingU16 + 1);
            Communication::addInt16(Communication::lastPingS16 + 1);
            break;
        }
        default:
        {
            Serial.print("Warn : There is nothing to send with the command 0x");
            Serial.println(Communication::lastCmd);
        }

    }
}
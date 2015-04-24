#include "Communication.h"


byte Communication::lastCmd = 0xff;
byte Communication::lastPingU8 = 0;
char Communication::lastPingS8 = 0;
unsigned short Communication::lastPingU16 = 0;
short Communication::lastPingS16 = 0;
Odometry* Communication::odometry = Odometry::getInst();
Enslavement* Communication::enslavement;

Encoder* Communication::leftEncoder = NULL;
Encoder* Communication::rightEncoder = NULL;
Motor* Communication::leftMotor = NULL;
Motor* Communication::rightMotor = NULL;


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
        case Communication::cmd_move:
        {
            if(length >= 5)
            {
                byte pos = 0;
                short x = extractInt16(&pos, params);
                short y = extractInt16(&pos, params);
                bool forceFace = extractUInt8(&pos, params);

                Communication::enslavement->turn(180);
            }
            break;
        }
        case Communication::cmd_turn:
        {
            if(length >= 2)
            {
                byte pos = 0;
                short angle = extractInt16(&pos, params);
                enslavement->turn(angle);
            }
            break;
        }
        case Communication::cmd_stop:
        {
            Communication::enslavement->stop();
            break;
        }
        case Communication::cmd_set_motor_pwm:
        {
            if(length >= 3)
            {
                byte pos = 0;
                byte motor = extractUInt8(&pos, params);
                short value = extractInt16(&pos, params);

                if(motor == 0 && Communication::leftMotor != NULL)
                    Communication::leftMotor->run((int) value);
                else if(motor == 1 && Communication::rightMotor != NULL)
                    Communication::rightMotor->run((int) value);
            }
            break;
        }
        case Communication::cmd_reset_encoder_ticks:
        {
            if(length >= 1)
            {
                byte pos = 0;
                byte motor = extractUInt8(&pos, params);

                if(motor == 0 && Communication::leftMotor != NULL)
                    Communication::leftEncoder->resetTicks();
                else if(motor == 1 && Communication::rightMotor != NULL)
                    Communication::rightEncoder->resetTicks();
            }
            break;
        }
        default:
        {
            // Serial.print("Warn : Command 0x");
            // Serial.print(command, HEX);
            // Serial.println(" doen't exist");
        }
    }
}

void Communication::send()
{
    switch(Communication::lastCmd)
    {
        case Communication::cmd_ping:
        {
            Serial.println("Send ping");
            Communication::addUInt8(Communication::lastPingU8 + 1);
            Communication::addInt8(Communication::lastPingS8 + 1);
            Communication::addUInt16(Communication::lastPingU16 + 1);
            Communication::addInt16(Communication::lastPingS16 + 1);
            break;
        }
        case Communication::cmd_odometry:
        {
            Serial.println("Send odometry");
            CarthesianCoordinates coordinates = Communication::odometry->getCoordinates();
            int orientation = Communication::odometry->getOrientation();

            Communication::addInt16(coordinates.x);
            Communication::addInt16(coordinates.y);
            Communication::addInt16(orientation);
            break;
        }
        case Communication::cmd_get_encoder_ticks:
        {
            if(Communication::leftEncoder != NULL && Communication::rightEncoder != NULL)
            {
                Communication::addInt16((short)Communication::leftEncoder->getTicks());
                Communication::addInt16((short)Communication::rightEncoder->getTicks());
            }
            break;
        }


        default:
        {
            Serial.print("Warn : There is nothing to send with the command 0x");
            Serial.println(Communication::lastCmd);
        }

    }
}

void Communication::setMotors(Motor* left, Motor* right)
{
    Communication::leftMotor = left;
    Communication::rightMotor = right;
}

void Communication::setEncoders(Encoder* left, Encoder* right)
{
    Communication::leftEncoder = left;
    Communication::rightEncoder = right;
}

void Communication::setEnslavement(Enslavement* enslavement)
{
    Communication::enslavement = enslavement;
}
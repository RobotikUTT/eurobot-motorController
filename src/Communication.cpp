#include "Communication.h"


byte Communication::lastCmd = 0xff;
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
            if(length >= 1)
            {
                byte pos = 0;
                byte val = extractUInt8(&pos, params);
                if(val != Communication::address)
                {
                    Serial.println("Warning : Bad ping address received !");
                }
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
                Communication::enslavement->goTo(x, y, forceFace);
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
        case Communication::cmd_set_tunings:
        {
            if (length >= 10)
            {
                byte pos = 0;
                float kp = extractFloat(&pos, params);
                float ki = extractFloat(&pos, params);
                float kd = extractFloat(&pos, params);
                float dt = extractUInt16(&pos, params);

                Communication::enslavement->setDeltaT(dt);

                // Only orientation PID for now, TODO distance PID
                Pid* distancePID = Communication::enslavement->getDistancePID();
                distancePID->setKp(kp);
                distancePID->setKi(ki);
                distancePID->setKd(kd);
                distancePID->setDeltaT(dt);
            }
        }
        case Communication::cmd_set_odometry:
        {
            if (length >= 6)
            {
                byte pos = 0;
                double x = (double)Communication::extractFloat(&pos, params);
                double y = (double)Communication::extractFloat(&pos, params);
                double orientation = (double)Communication::extractFloat(&pos, params);

                Communication::odometry->setOrientation(orientation);
                Communication::odometry->setCoordinates(x,y);
            }
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
            Communication::addUInt8(Communication::address);
            break;
        }
        case Communication::cmd_get_odometry:
        {
            if(Communication::odometry != NULL)
            {
                CarthesianCoordinates coordinates = Communication::odometry->getCoordinates();

                Communication::addFloat((float)coordinates.x);
                Communication::addFloat((float)coordinates.y);
                Communication::addFloat((float)Communication::odometry->getOrientation());
            }
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

void Communication::setOdometry(Odometry* odometry)
{
    Communication::odometry = odometry;
}
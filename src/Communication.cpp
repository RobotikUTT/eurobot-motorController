#include "Communication.h"


byte Communication::lastCmd = 0xff;
Odometry* Communication::odometry = Odometry::getInst();
Enslavement* Communication::enslavement;

Encoder* Communication::leftEncoder = NULL;
Encoder* Communication::rightEncoder = NULL;
Motor* Communication::leftMotor = NULL;
Motor* Communication::rightMotor = NULL;

void Communication::open(byte address, byte dataAvailablePin, char dataAvailableCmd)
{
    Communication::dataAvailableCmd = dataAvailableCmd;
    Communication::dataAvailablePin = dataAvailablePin;
    pinMode(dataAvailablePin, OUTPUT);
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
                    Serial.print("Warning : Bad ping address received : 0x");
                    Serial.print(val, HEX);
                    Serial.print(" instead of 0x");
                    Serial.println(Communication::address, HEX);
                }
            }
            break;
        }
        case Communication::cmd_move:
        {
            if(length >= 4)
            {
                byte pos = 0;
                double distance = extractFloat(&pos, params);
                Communication::enslavement->go(distance);
                digitalWrite(Communication::dataAvailablePin, LOW);
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
                digitalWrite(Communication::dataAvailablePin, LOW);
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
                float orientationKp = extractFloat(&pos, params);
                float orientationKi = extractFloat(&pos, params);
                float orientationKd = extractFloat(&pos, params);
                float distanceKp = extractFloat(&pos, params);
                float distanceKi = extractFloat(&pos, params);
                float distanceKd = extractFloat(&pos, params);
                float dt = extractUInt16(&pos, params);

                Communication::enslavement->setDeltaT(dt);

                Pid* orientationPID = Communication::enslavement->getOrientationPID();
                orientationPID->setKp(orientationKp);
                orientationPID->setKi(orientationKi);
                orientationPID->setKd(orientationKd);
                orientationPID->setDeltaT(dt);
                Pid* distancePID = Communication::enslavement->getDistancePID();
                distancePID->setKp(distanceKp);
                distancePID->setKi(distanceKi);
                distancePID->setKd(distanceKd);
                distancePID->setDeltaT(dt);
            }
            break;
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
            break;
        }
        case Communication::cmd_set_mode:
        {
            if (length >=  1)
            {
                byte pos = 0;
                EnslavementMode mode;

                switch (Communication::extractUInt8(&pos, params))
                {
                    case 0:
                        mode = DEBUG_ORIENTATION;
                        break;

                    case 1:
                        mode = DEBUG_DISTANCE;
                        break;

                    case 2:
                        mode = POLAR;
                        break;

                    default:
                        mode = POLAR;
                        break;
                }

                Communication::enslavement->setMode(mode);
                break;
            }
        }
        case Communication::cmd_start_entraxe:
        {
            Communication::odometry->ENTRAXE = 0;
            Communication::enslavement->setEntraxe = true;
            break;
        }
        case Communication::cmd_stop_entraxe:
        {
            Communication::enslavement->setEntraxe = false;
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
                Ticks ticks = odometry->getTicks();
                Communication::addInt16((short)ticks.left);
                Communication::addInt16((short)ticks.right);
            }
            break;
        }
        case Communication::cmd_stop_entraxe:
        {
            Communication::addFloat(Communication::odometry->ENTRAXE);
            Communication::odometry->ENTRAXE = Odometry::ticksToMeters(Communication::odometry->ENTRAXE);
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
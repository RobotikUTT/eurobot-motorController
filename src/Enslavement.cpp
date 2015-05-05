#include "Enslavement.h"


Enslavement::Enslavement(unsigned long deltaT, double acceleration, double velocityMax,
    Motor *leftMotor, Motor *rightMotor)
{
    this->odometry = Odometry::getInst(NULL, NULL);
    this->leftMotor = leftMotor;
    this->rightMotor = rightMotor;
    this->distancePID = new Pid(0.9, 0, 0.1, deltaT);
    this->orientationPID = new Pid(2.3, 0, 0.25, deltaT);

    this->deltaT = deltaT;
    this->lastMillis = 0;


    this->distanceVelocityMax = Odometry::metersToTicks(velocityMax / 1000 * deltaT); //m.s^-1 => ticks.deltaT^-1
    this->distanceAcceleration = Odometry::metersToTicks(acceleration / 1000 / 1000 * deltaT * deltaT); //m.s^-2 => ticks.deltaT^-2
    this->orientationVelocityMax = this->distanceVelocityMax / Odometry::ENTRAXE / 2.0;
    this->orientationAcceleration = this->distanceAcceleration / Odometry::ENTRAXE / 2.0;

    this->actualDistance = 0;
    this->actualOrientation = 0;

    this->orientationObjective = 0;
    this->distanceObjective = 0;
    this->running = false;
    this->mode = POLAR;
    this->orientationOffset = 0;
}


Enslavement::~Enslavement() { }


void Enslavement::go(double distance)
{
    this->running = true;
    this->actualDistance = 0;
    this->distanceObjective = Odometry::metersToTicks(distance);
    this->orientationObjective = 0;
}


void Enslavement::turn(double theta)
{
    this->running = true;
    // this->orientationOffset = Odometry::metersToTicks(this->odometry->getOrientation() * Odometry::ENTRAXE);
    this->orientationObjective = Odometry::metersToTicks(theta * 0.0174532925 * Odometry::ENTRAXE);
    this->distanceObjective = 0 ;
}


void Enslavement::compute()
{
    unsigned long now = millis();
    unsigned int timeElapsed = (now - this->lastMillis);

    if (timeElapsed >= this->deltaT)
    {
        this->lastMillis = now;

        this->odometry->update();

        Ticks ticks = this->odometry->getTicks();

        this->odometry->getLeftEncoder()->resetTicks();
        this->odometry->getRightEncoder()->resetTicks();

        this->actualDistance += (ticks.left + ticks.right) / 2;
        double actualOrientation = ticks.left - ticks.right;
        // double actualOrientation = Odometry::metersToTicks(this->odometry->getOrientation() * Odometry::ENTRAXE) - this->orientationOffset;

        if (!this->running) {
            return;
        }

        double remainingDistance = this->distanceObjective - actualDistance;
        double remainingOrientation = fmod(this->orientationObjective - actualOrientation,
            Odometry::metersToTicks(2 * M_PI * Odometry::ENTRAXE));

        Serial.println(remainingDistance, 5);
        Serial.println(remainingOrientation, 5);

        if (fabs(remainingOrientation) > Odometry::metersToTicks(M_PI * Odometry::ENTRAXE))
        {
            int sign = (remainingOrientation > 0) - (remainingOrientation < 0);
            remainingOrientation -= sign * Odometry::metersToTicks(2 * M_PI * Odometry::ENTRAXE);
        }

        /*
            Error correction
        */

        double distanceCommand = this->distancePID->compute(remainingDistance);
        double orientationCommand = this->orientationPID->compute(remainingOrientation);

        if (this->mode == DEBUG_ORIENTATION)
        {
            distanceCommand = 0;
        }
        else if (this->mode == DEBUG_DISTANCE)
        {
            orientationCommand = 0;
        }

        double leftCommand = distanceCommand+orientationCommand;
        double rightCommand = distanceCommand-orientationCommand;

        this->leftMotor->run(leftCommand);
        this->rightMotor->run(rightCommand);
    }
}


void Enslavement::stop()
{
    this->leftMotor->stop();
    this->rightMotor->stop();

    this->running = false;
}


Pid* Enslavement::getOrientationPID()
{
    return this->orientationPID;
}


Pid* Enslavement::getDistancePID()
{
    return this->distancePID;
}


void Enslavement::setDeltaT(unsigned long deltaT)
{
    this->deltaT = deltaT;
}


void Enslavement::setMode(EnslavementMode newMode)
{
    this->mode = newMode;
}
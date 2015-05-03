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

    this->theoricalDistance = 0;
    this->theoricalDistanceVelocity = 0;

    this->theoricalOrientation = 0;
    this->theoricalOrientationVelocity = 0;

    this->orientationObjective = 0;
    this->distanceObjective = 0;
    this->running = false;
    this->mode = POLAR;
}


Enslavement::~Enslavement() { }


void Enslavement::goTo(double x, double y, bool forceFace)
{
    this->running = true;
    CarthesianCoordinates coordinates = this->odometry->getCoordinates();
    CarthesianCoordinates newCoordinates = {x - coordinates.x, y - coordinates.y};
    this->orientationObjective = atan2(newCoordinates.y, newCoordinates.x) - this->odometry->getOrientation();

    int face = 1;
    if (!forceFace && fabs(this->orientationObjective) >= M_PI)
    {
        int sign = (this->orientationObjective >= 0) ? 1 : -1;
        this->orientationObjective -= sign * M_PI;
        face = -1;
    }

    this->distanceObjective = Odometry::metersToTicks(face * sqrt(pow(newCoordinates.x, 2) + pow(newCoordinates.y, 2)));
    this->orientationObjective = 0;
}


void Enslavement::turn(double theta)
{
    this->running = true;
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
        CarthesianCoordinates coordinates = this->odometry->getCoordinates();

        this->odometry->getLeftEncoder()->resetTicks();
        this->odometry->getRightEncoder()->resetTicks();

        double actualDistance = Odometry::metersToTicks(sqrt(pow(coordinates.x, 2) + pow(coordinates.y, 2)));
        double actualOrientation = Odometry::metersToTicks(odometry->getOrientation() * Odometry::ENTRAXE);

        this->previousDistance = actualDistance;
        this->previousOrientation = actualOrientation;

        if (!this->running) {
            return;
        }

        double remainingDistance = this->distanceObjective - actualDistance;
        double remainingOrientation = fmod(this->orientationObjective - actualOrientation,
            Odometry::metersToTicks(2 * M_PI * Odometry::ENTRAXE));

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
            Serial.println("orientation");
            distanceCommand = 0;
        }
        else if (this->mode == DEBUG_DISTANCE)
        {
            Serial.println("distance");
            orientationCommand = 0;
        }
        else
        {
            Serial.println("sida");
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

    this->theoricalOrientation = 0;
    this->orientationVelocityObjective = 0;
    this->previousOrientation = 0;
    this->theoricalOrientationVelocity = 0;

    this->theoricalDistance = 0;
    this->distanceVelocityObjective = 0;
    this->previousDistance = 0;
    this->theoricalDistanceVelocity = 0;

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
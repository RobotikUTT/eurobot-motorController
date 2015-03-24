#include "Enslavement.h"


Enslavement::Enslavement(unsigned long deltaT, double acceleration, double velocityMax,
    Motor *leftMotor, Motor *rightMotor)
{
    this->odometry = Odometry::getInst(NULL, NULL);
    this->leftMotor = leftMotor;
    this->rightMotor = rightMotor;
    this->distancePID = new Pid(0.01, 0.1, 0, deltaT);
    this->orientationPID = new Pid(0.05, 0, 0, deltaT);

    this->deltaT = deltaT;
    this->lastMillis = 0;


    this->distanceVelocityMax = Odometry::metersToTicks(velocityMax / 1000 * deltaT); //m.s^-1 => m.deltaT^-1
    this->distanceAcceleration = Odometry::metersToTicks(acceleration / 1000 / 1000 * deltaT * deltaT); //m.s^-2 => m.deltaT^-2
    this->orientationVelocityMax = this->distanceVelocityMax / Odometry::ENTRAXE / 2.0;
    this->orientationAcceleration = this->distanceAcceleration / Odometry::ENTRAXE / 2.0;

    this->previousDistance = 0;
    this->previousOrientation = 0;

    this->distanceObjective = 0;
    this->distanceVelocityObjective = 0;
    this->orientationObjective = 0;
    this->orientationVelocityObjective = 0;

    this->theoricalDistance = 0;
    this->theoricalDistanceVelocity = 0;
    this->theoricalOrientation = 0;
    this->theoricalOrientationVelocity = 0;
}


Enslavement::~Enslavement() { }


void Enslavement::setDeltaT(int deltaT)
{
    if (deltaT > 0)
    {
        this->deltaT = deltaT;
    }
}


void Enslavement::goTo(double x, double y, bool forceFace)
{
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
}


void Enslavement::turn(double theta)
{
    this->orientationObjective = theta * Encoder::TICK_PER_SPIN / 360;
}


void Enslavement::compute()
{
    unsigned long now = millis();
    unsigned int timeElapsed = (now - this->lastMillis);
    // unsigned int timeElapsed = this->deltaT;

    if (timeElapsed >= this->deltaT)
    {
        this->lastMillis = now;

        this->odometry->update();
        Ticks ticks = this->odometry->getTicks();

        double actualDistance = this->previousDistance + (ticks.left + ticks.right) / 2.0;
        double actualDistanceVelocity = actualDistance - this->previousDistance;

        double actualOrientation = this->previousOrientation + (ticks.right - ticks.left) / Odometry::ENTRAXE / 2.0;
        double actualOrientationVelocity = actualOrientation - this->previousOrientation;

        this->previousDistance = actualDistance;
        this->previousOrientation = actualOrientation;

        this->odometry->getLeftEncoder()->resetTicks();
        this->odometry->getRightEncoder()->resetTicks();

        /*
            Generate next point
        */

        double remainingDistance = this->distanceObjective - actualDistance;
        double remainingOrientation = this->orientationObjective - actualOrientation;

        if (fabs(remainingDistance) <= this->distanceAcceleration)
        {
            this->distanceVelocityObjective = 0;
            this->distanceObjective = 0;
            this->previousDistance = 0;
        }
        else
        {
            if ((fabs(remainingDistance) <=  fabs(pow(actualDistanceVelocity, 2) /( 2 * this->distanceAcceleration))))
            {
                this->distanceVelocityObjective = 0;
            }
            else if (remainingDistance > 0)
            {
                this->distanceVelocityObjective = this->distanceVelocityMax;
            }
            else if (remainingDistance < 0)
            {
                this->distanceVelocityObjective = -(this->distanceVelocityMax);
            }
        }

        if ((int)((this->theoricalDistanceVelocity + this->distanceAcceleration)/0.01)*0.01 <= this->distanceVelocityObjective)
        {
            this->theoricalDistanceVelocity += this->distanceAcceleration;
        }
        else if ((int)((this->theoricalDistanceVelocity - this->distanceAcceleration)/0.01)*0.01 >= this->distanceVelocityObjective)
        {
            this->theoricalDistanceVelocity -= this->distanceAcceleration;
        }

        this->theoricalDistance += this->theoricalDistanceVelocity;


        if (fabs(remainingOrientation) <= this->orientationAcceleration)
        {
            this->orientationVelocityObjective = 0;
            this->orientationObjective = 0;
            this->previousOrientation = 0;
        }
        else
        {
            if ((fabs(remainingOrientation) <=  fabs(pow(actualOrientationVelocity, 2) /( 2 * this->orientationAcceleration))))
            {
                this->orientationVelocityObjective = 0;
            }
            else if (remainingOrientation > 0)
            {
                this->orientationVelocityObjective = this->orientationVelocityMax;
            }
            else if (remainingOrientation < 0)
            {
                this->orientationVelocityObjective = -(this->orientationVelocityMax);
            }
        }

        if ((int)((this->theoricalOrientationVelocity + this->orientationAcceleration)/0.01)*0.01 <= this->orientationVelocityObjective)
        {
            this->theoricalOrientationVelocity += this->orientationAcceleration;
        }
        else if ((int)((this->theoricalOrientationVelocity - this->orientationAcceleration)/0.01)*0.01 >= this->orientationVelocityObjective)
        {
            this->theoricalOrientationVelocity -= this->orientationAcceleration;
        }

        this->theoricalOrientation += this->theoricalOrientationVelocity;

        /*
            Error correction
        */

        double distanceCommand = this->distancePID->compute(actualDistanceVelocity, this->theoricalDistanceVelocity);
        double orientationCommand = this->orientationPID->compute(actualOrientationVelocity, this->theoricalOrientationVelocity);

        double leftCommand = distanceCommand + orientationCommand;
        double getRightEncoder = distanceCommand - orientationCommand;

        // Serial.print("Rem ");
        // Serial.println(remainingOrientation);
        // Serial.print("Th ");
        // Serial.println(theoricalOrientationVelocity);
        // Serial.print("Rl ");
        // Serial.println(actualOrientationVelocity);
        // Serial.println("___");
        Serial.println(remainingOrientation);
        this->leftMotor->run((int)orientationCommand);
        this->rightMotor->run((int)-orientationCommand);
    }
}
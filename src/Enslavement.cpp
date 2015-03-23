#include "Enslavement.h"


Enslavement::Enslavement(unsigned long deltaT, double acceleration, double velocityMax,
    Motor *leftMotor, Motor *rightMotor)
{
    this->odometry = Odometry::getInst(NULL, NULL);
    this->leftMotor = leftMotor;
    this->rightMotor = rightMotor;
    this->distancePID = new Pid(0.01, 0.1, 0, deltaT);

    this->deltaT = deltaT;
    this->lastMillis = 0;


    this->distanceAcceleration = Odometry::metersToTicks(acceleration / 1000 / 1000 * deltaT * deltaT); //m.s^-2 => m.deltaT^-2
    this->distanceVelocityMax = Odometry::metersToTicks(velocityMax / 1000 * deltaT); //m.s^-1 => m.deltaT^-1

    this->distanceObjective = 0;
    this->theoricalDistance = 0;

    this->velocityObjective = 0;
    this->theoricalVelocity = 0;

    this->previousDistance = 0;
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
    //TODO
}


void Enslavement::compute()
{
    unsigned long now = millis();
    // unsigned int timeElapsed = (now - this->lastMillis);
    unsigned int timeElapsed = this->deltaT;

    if (timeElapsed >= this->deltaT)
    {
        this->lastMillis = now;

        this->odometry->update();
        Ticks ticks = this->odometry->getTicks();

        CarthesianCoordinates coordinates = this->odometry->getCoordinates();

        double actualDistance = this->previousDistance + (ticks.left + ticks.right) / 2.0;
        double actualDistanceVelocity = actualDistance - previousDistance;

        this->previousDistance = actualDistance;

        /*
            Generate next point
        */

        double remainingDistance = this->distanceObjective - actualDistance;

        /*
            Debug
            TODO: ifef DEBUG, find a way to properly deploy debug or release
         */

        Serial.print(actualDistanceVelocity);
        Serial.print(",");
        // Serial.print("remainingDistance: ")
        // Serial.println(remainingDistance);
        //Serial.print("velocityOjective :");
        //Serial.println(this->velocityObjective);
        // Serial.print(this->theoricalVelocity);
        // Serial.print(",");
        // Serial.print("Real velocity: ");
        // Serial.println(actualDistanceVelocity);
        //Serial.println("----------------------------");

        if (fabs(remainingDistance) <= this->distanceAcceleration)
        {
            this->velocityObjective = 0;
            this->distanceObjective = 0;
            this->previousDistance = 0;
            this->leftMotor->stop();
            this->rightMotor->stop();
        }
        else
        {
            if ((fabs(remainingDistance) <=  fabs(pow(theoricalVelocity, 2) /( 2 * this->distanceAcceleration))))
            {
                /*
                Serial.println("Deccelerate !");
                Serial.print("remaining: ");
                Serial.print(fabs(remainingDistance));
                Serial.print(" vs: ");
                Serial.println(fabs(pow(this->theoricalVelocity, 2) /( 2 * this->distanceAcceleration)));*/
                // Serial.print(remainingDistance);
                // Serial.print(" <= ");
                // Serial.println(pow(this->actualDistanceVelocity, 2) /( 2 * this->distanceAcceleration));
                this->velocityObjective = 0;
            }
            else if (remainingDistance > 0)
            {
                this->velocityObjective = this->distanceVelocityMax;
            }
            else if (remainingDistance < 0)
            {
                this->velocityObjective = -(this->distanceVelocityMax);
            }
        }

        if ((int)((this->theoricalVelocity + this->distanceAcceleration)/0.01)*0.01 <= this->velocityObjective)
        {
            this->theoricalVelocity += this->distanceAcceleration;
        }
        else if ((int)((this->theoricalVelocity - this->distanceAcceleration)/0.01)*0.01 >= this->velocityObjective)
        {
            this->theoricalVelocity -= this->distanceAcceleration;
        }

        this->theoricalDistance += this->theoricalVelocity;


        /*
            Error correction
        */

        double distanceCommand = this->distancePID->compute(actualDistanceVelocity, this->theoricalVelocity);
        this->leftMotor->run((int)distanceCommand);
        this->rightMotor->run((int)distanceCommand);
    }
}
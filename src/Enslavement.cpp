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


    this->distanceAcceleration = Odometry::metersToTicks((acceleration / (1000 * 1000)) * deltaT * deltaT); //m.s^-2 => m.deltaT^-2
    this->distanceVelocityMax = Odometry::metersToTicks((velocityMax / 1000) * deltaT); //m.s^-1 => m.deltaT^-1

    this->distanceObjective = 0;
    this->theoricalDistance = 0;
}


Enslavement::~Enslavement()
{

}


void Enslavement::setDeltaT(int deltaT)
{
    if (deltaT > 0)
        this->deltaT = deltaT;
}


void Enslavement::goTo(CarthesianCoordinates newCoordinates)
{
    CarthesianCoordinates coordinates = this->odometry->getCoordinates();
    double newDistance = sqrt(pow(newCoordinates.x, 2) + pow(newCoordinates.y, 2));
    double newAngle = 0;

    if (newCoordinates.x - coordinates.x != 0)
    {
        newAngle = atan2(newCoordinates.y - coordinates.y, newCoordinates.x - coordinates.x);
    }

    this->distanceObjective = Odometry::metersToTicks(newDistance);
}


void Enslavement::turn(double theta)
{
}


void Enslavement::compute()
{
        this->odometry->update();
        CarthesianCoordinates coordinates = this->odometry->getCoordinates();

        this->actualDistance = Odometry::metersToTicks(sqrt(pow(coordinates.x, 2) + pow(coordinates.y, 2)));
        actualDistanceVelocity = this->actualDistance - this->previousDistance;


        this->previousDistance = this->actualDistance;

        /*
            Generate next point
        */

        double remainingDistance = this->distanceObjective - this->actualDistance;
        Serial.print(this->actualDistanceVelocity);
        Serial.print(",");
        // std::cout << this->theoricalVelocity << " vs " << actualDistanceVelocity << std::endl;
        // std::cout << "error: " << this->theoricalVelocity - actualDistanceVelocity << std::endl;

        bool done = false;
        if (fabs(remainingDistance) <= this->distanceAcceleration)
        {
            done = true;
        }

        if (done)
        {
            done = false;
        }
        else
        {
            if ((remainingDistance <=  pow(this->theoricalVelocity, 2) /( 2 * this->distanceAcceleration)))
            {
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

        if ((int)(this->theoricalVelocity/0.01)*0.01 == this->velocityObjective)
        {
            this->theoricalVelocity = this->velocityObjective;
        }
        else if ((int)((this->theoricalVelocity + this->distanceAcceleration)/0.01)*0.01 <= this->velocityObjective)
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

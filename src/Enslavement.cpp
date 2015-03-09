#include "Enslavement.h"


Enslavement::Enslavement(unsigned long deltaT, double acceleration, double velocityMax, 
    Motor *leftMotor, Motor *rightMotor)
{
    // Dirty work with this singleton. see motorController.ino for more explanation
    this->odometry = Odometry::getInst(NULL, NULL);

    this->leftMotor = leftMotor;
    this->rightMotor = rightMotor;

    this->distancePID = new Pid(1, 0, 0, this->deltaT);
    this->anglePID = new Pid(1, 0, 0, this->deltaT);


    this->deltaT = deltaT;
    this->orientationAcceleration = Odometry::metersToTicks(2 * acceleration / Odometry::ENTRAXE);
    this->distanceAcceleration = Odometry::metersToTicks(acceleration);
    this->orientationVelocityMax = Odometry::metersToTicks(2 * velocityMax / Odometry::ENTRAXE);   // / entraxe
    this->distanceVelocityMax = Odometry::metersToTicks(velocityMax);
    this->orientationGain = this->orientationAcceleration * deltaT * deltaT;
    this->distanceGain = this->distanceAcceleration * deltaT * deltaT;


    this->lastMillis = 0;
    this->orientationObjective = 0;
    this->distanceObjective = 0;
    this->theoricalDistance = 0;
    this->theoricalOrientation = 0;
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
    this->orientationObjective = Odometry::metersToTicks(newAngle);
}


void Enslavement::turn(double theta)
{
    this->orientationObjective = theta;
}


void Enslavement::compute()
{
    unsigned long now = millis();
    unsigned long timeElapsed = (now - this->lastMillis);

    if (timeElapsed >= this->deltaT)
    {
        this->lastMillis = now;

        /**
         * Odometry data
         */
        
        //Old data
        CarthesianCoordinates coordinates = this->odometry->getCoordinates();
        double lastDistance = Odometry::metersToTicks(sqrt(pow(coordinates.x, 2) + pow(coordinates.y, 2)));
        double lastOrientation = this->odometry->getOrientation();

        //Update data
        this->odometry->update();
        coordinates = this->odometry->getCoordinates();
        double distance = Odometry::metersToTicks(sqrt(pow(coordinates.x, 2) + pow(coordinates.y, 2)));
        double orientation = this->odometry->getOrientation();

        double remainingDistance = this->distanceObjective - distance;
        double remainingOrientation = this->orientationObjective - orientation;
        double distanceTraveled = distance - lastDistance;
        double orientationTraveled = orientation - lastOrientation;


        /**
         * Enslavement
         */

        //Distance 
        
        //Shall we decelerate ?
        if (remainingDistance <= 0.5 * pow(distanceTraveled / this->deltaT, 2) / this->distanceAcceleration)
        {
            this->distanceObjective -= this->distanceGain;
        }
        //Or accelerate ?
        else if (distanceTraveled / this->deltaT < this->distanceVelocityMax)
        {
            this->distanceObjective += this->distanceGain;
        }

        //Angle
        if (remainingOrientation <= 0.5 * pow(orientationTraveled / this->deltaT, 2) / this->orientationAcceleration)
        {
            this->orientationObjective -= this->orientationGain;
        }
        else if (orientationTraveled / this->deltaT < this->orientationVelocityMax)
        {
            this->orientationObjective += this->orientationGain;
        }


        /**
         * Error correction
         */

         double distanceCommand = this->distancePID->compute(distance, this->distanceObjective);
         double orientationCommand = this->distancePID->compute(distance, this->orientationObjective);


        /**
         * Motor control
         */
        
        int leftCommand = distanceCommand - orientationCommand;
        int rightCommand = distanceCommand + orientationCommand;

        this->leftMotor->run(leftCommand);
        this->rightMotor->run(rightCommand);
    }
}
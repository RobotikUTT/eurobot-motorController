#include "Enslavement.h"


Enslavement::Enslavement(unsigned long deltaT, double acceleration, double velocityMax, 
    Motor *leftMotor, Motor *rightMotor)
{
    this->distanceAcceleration = Odometry::metersToTicks(acceleration);
    this->distanceVelocityMax = Odometry::metersToTicks(velocityMax);
    this->orientationAcceleration = 2 * acceleration; // / entraxe
    this->orientationVelocityMax = 2 * velocityMax;      // same

    this->velocityGain = acceleration * deltaT;

    this->leftMotor = leftMotor;
    this->rightMotor = rightMotor;

    this->deltaT = deltaT;
    this->lastMillis = 0;

    this->distanceObjective = 0;
    this->theoricalDistance = 0;
    this->currentDistance = 0;
    this->lastDistance = 0;
    this->distanceCommand = 0;

    this->orientationObjective = 0;
    this->theoricalOrientation = 0;
    this->currentOrientation = 0;
    this->lastOrientation = 0;
    this->orientationCommand = 0;

    //Dirty work with this singleton. see motorController.ino for more explanation
    this->odometry = Odometry::getInst(NULL, NULL);

    this->distancePID = new Pid(1, 0, 0, this->deltaT, &(this->currentDistance), 
        &(this->theoricalDistance), &(this->distanceCommand));
    this->anglePID = new Pid(1, 0, 0, this->deltaT, &(this->currentOrientation), 
        &(this->orientationObjective), &(this->orientationCommand));
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

    double distance = sqrt(pow(newCoordinates.x, 2) + pow(newCoordinates.y, 2));
    double angle;

    if (newCoordinates.x - coordinates.x != 0)
    {
        angle = atan2(newCoordinates.y - coordinates.y, newCoordinates.x - coordinates.x);
    }
    else
    {
        angle = 0;
    }

    //this->distanceObjective = Odometry::metersToTicks(distance);
    this->orientationObjective = angle;
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

        //Actual velocity is deduced by the number of encoder ticks ellapsed since the last call of execute
        long actualVelocity;

        //Update odometry
        this->odometry->update();

        Ticks ticks = this->odometry->getTicks();
        double orientation = this->odometry->getOrientation();

        long leftTicks = ticks.left;
        long rightTicks = ticks.right;


        /*
            Distance enslavement
         */
        
        /*
         * Distance enslavement will be tested after angle enslavement
         * 
         *
        this->currentDistance = (leftTicks + rightTicks) / 2;

        //Update theorical trajectory
        double remainingDistance = this->distanceObjective - currentDistance;

        //Precision reached
        if (Odometry::ticksToMeter(abs(remainingDistance)) <= 0.1)
        {
            return;
        }

        double actualDistanceVelocity = (this->lastDistance - currentDistance) / this->deltaT * 1000;
        double theoricalDistanceVelocity = this->theoricalDistance / this->deltaT;

        if (this->theoricalDistance > 0 && (remainingDistance/this->theoricalDistanceVelocity) <= (theoricalDistanceVelocity/this->distanceAcceleration))
        {
            //Time to decelerate
            this->theoricalDistance -= this->distanceAcceleration * this->deltaT * this->deltaT;
        }
        else if (this->theoricalDistance/this->deltaT < this->distanceVelocityMax)
        {
            //Time to accelerate
            this->theoricalDistance += this->distanceAcceleration * this->deltaT * this->deltaT;
        }

        //PID
        this->distancePID->compute();    

         */    

        /*
            Angle enslavement
         */
        
        this->currentOrientation = orientation;

        //Update theorical trajectory
        double remainingOrientation = this->orientationObjective - currentOrientation;
     
        //Precision reached
        if (abs(remainingOrientation) <= 0.2)
        {
            return;
        }

        double actualOrientationVelocity = (this->lastOrientation - currentOrientation) / this->deltaT * 1000;
        double theoricalOrientationVelocity = this->theoricalOrientation / this->deltaT;
        
        if (this->theoricalOrientation != 0 && (remainingOrientation/theoricalOrientationVelocity) <= (theoricalOrientationVelocity/this->orientationAcceleration))
        {
            //Time to decelerate
            this->theoricalOrientation -= this->orientationAcceleration * this->deltaT * this->deltaT;
        }
        else if (this->theoricalOrientation/this->deltaT < this->orientationVelocityMax)
        {
            //Time to accelerate
            this->theoricalOrientation += this->orientationAcceleration * this->deltaT * this->deltaT;
        }

        //PID
        this->anglePID->compute();


        /*
            Motor command
         */
        
        int leftCommand = this->distanceCommand - this->orientationCommand;
        int rightCommand = this->distanceCommand + this->orientationCommand;

        this->leftMotor->run(leftCommand);
        this->rightMotor->run(rightCommand);

        #ifdef DEBUG
            Serial.print("orientationCommand: ");
            Serial.println(this->orientationCommand);
            Serial.print("Actual angle velocity:");
            Serial.println(actualOrientationVelocity)
        #endif
    }
}
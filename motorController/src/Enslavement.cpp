#include "Enslavement.h"


Enslavement::Enslavement(unsigned long deltaT, double acceleration, double velocityMax, 
    Motor *leftMotor, Motor *rightMotor)
{
    this->distanceAcceleration = Odometry::metersToTicks(acceleration);
    this->distanceVelocityMax = Odometry::metersToTicks(velocityMax);
    this->angleAcceleration = 2 * acceleration; // / entraxe
    this->angleVelocityMax = 2 * velocityMax;      // same

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

    this->angleObjective = 0;
    this->theoricalAngle = 0;
    this->currentAngle = 0;
    this->lastAngle = 0;
    this->angleCommand = 0;

    //Dirty work with this singleton. see motorController.ino for more explanation
    this->odometry = Odometry::getInst(NULL, NULL);

    this->distancePID = new Pid(1, 0, 0, this->deltaT, &(this->currentDistance), 
        &(this->theoricalDistance), &(this->distanceCommand));
    this->anglePID = new Pid(1, 0, 0, this->deltaT, &(this->currentAngle), 
        &(this->angleObjective), &(this->angleCommand));
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
    this->angleObjective = angle;
}


void Enslavement::turn(double theta)
{
    this->angleObjective = theta;
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
        
        this->currentAngle = (leftTicks - rightTicks);

        //Update theorical trajectory
        double remainingAngle = this->angleObjective - currentAngle;
     
        //Precision reached
        if (abs(remainingAngle) <= 0.2)
        {
            return;
        }

        double actualAngleVelocity = (this->lastAngle - currentAngle) / this->deltaT * 1000;
        double theoricalAngleVelocity = this->theoricalAngle / this->deltaT;
        
        if (this->theoricalAngle != 0 && (remainingAngle/theoricalAngleVelocity) <= (theoricalAngleVelocity/this->angleAcceleration))
        {
            //Time to decelerate
            this->theoricalAngle -= this->angleAcceleration * this->deltaT * this->deltaT;
        }
        else if (this->theoricalAngle/this->deltaT < this->angleVelocityMax)
        {
            //Time to accelerate
            this->theoricalAngle += this->angleAcceleration * this->deltaT * this->deltaT;
        }

        //PID
        this->anglePID->compute();


        /*
            Motor command
         */
        
        int leftCommand = this->distanceCommand - this->angleCommand;
        int rightCommand = this->distanceCommand + this->angleCommand;

        this->leftMotor->run(leftCommand);
        this->rightMotor->run(rightCommand);

        #ifdef DEBUG
            Serial.print("angleCommand: ");
            Serial.println(this->angleCommand);
            Serial.print("Actual angle velocity:");
            Serial.println(actualAngleVelocity)
        #endif
    }
}
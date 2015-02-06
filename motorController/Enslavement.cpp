#include "Enslavement.h"


Enslavement::Enslavement(unsigned long deltaT, double acceleration, double velocityMax, 
    Motor *leftMotor, Motor *rightMotor)
{
    this->acceleration = acceleration / 100;
    this->velocityMax = velocityMax / 100;
   
    this->velocityGain = acceleration * deltaT;
    this->accelerationTime = this->velocityMax / this->acceleration;

    this->leftMotor = leftMotor;
    this->rightMotor = rightMotor;

    this->deltaT = deltaT;
    this->lastMillis = 0;

    this->distanceObjective = 0;
    this->currentDistance = 0;
    this->distanceCommand = 0;

    this->angleObjective = 0;
    this->currentAngle = 0;
    this->angleCommand = 0;

    this->Odometry = Odometry::getInst();

    this->distancePID = new PID(1, 0, 0, this->deltaT, &(this->currentDistance), 
        &(this->distanceObjective), &(this->distanceCommand));
    this->anglePID = new PID(1, 0, 0, this->deltaT, &(this->currentAngle), 
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


void Enslavement::goTo(long newX, long newY)
{
    double distance = sqrt(pow(newX, 2) + pow(newY, 2));
    double angle;

    if (newX - x != 0)
    {
        angle = atan2(y, x);
    }
    else
    {
        angle = 0;
    }

    this->distanceObjective = Odometry::metersToTicks(distance);
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
        this->lastMillis = millis_;

        //Actual velocity is deduced by the number of encoder ticks ellapsed since the last call of execute
        long actualVelocity;

        //Update odometry
        this->odometry->update();

        Ticks ticks = this->odometry->getTicks();

        long leftTicks = ticks.left;
        long rightTicks = ticks.right;

        #ifdef DEBUG
            Serial.println("\n\n");
            Serial.print("leftTicks: ");
            Serial.println(leftTicks);
            Serial.print("rightTicks: ");
            Serial.println(rightTicks);
        #endif





        /*
            Distance velocity enslavement
         */
        
        long distanceVelocity = (leftTicks + rightTicks) / (2 * (long) timeElapsed);
        Serial.print("distance velocity: ");
        Serial.println(distanceVelocity);

        //PID
        long distanceVelocityError = this->distanceVelocityObjective - distanceVelocity;
        int distanceCommand = distanceVelocityError * this->kp - distanceVelocity * this->kd;

        //Update objectives
        long remainingDistance = this->distanceObjective - (leftTicks + rightTicks)/2;

        if (remainingDistance/distanceVelocity <= this->accelerationTime)
        {
            //Time to decelerate
            this->distanceVelocityObjective -= this->velocityGain;
        }
        else if (this->distanceVelocityObjective < this->velocityMax)
        {
            //Time to accelerate
            this->distanceVelocityObjective += this->velocityGain;
        }


        /*
            Angle velocity enslavement
         */
        
        long angleVelocity = (leftTicks - rightTicks) / (long) timeElapsed;
        Serial.print("angle velocity: ");
        Serial.println(angleVelocity);

        //PID
        long anglevelocityError = this->angleVelocityObjective - angleVelocity;
        int angleCommand = angleVelocityObjective * this->kp - angleVelocity * this->kd;

        //Update objectives
        long remainingAngle = this->orientationObjective - (leftTicks - rightTicks);
        
        if (remainingAngle/angleVelocity <= this->accelerationTime)
        {
            //Time to decelerate
            this->angleVelocityObjective -= this->velocityGain;
        }
        else if (this->angleVelocityObjective < this->velocityMax)
        {
            //Time to accelerate
            this->angleVelocityObjective += this->velocityGain;
        }


        int leftCommand = distanceCommand - angleCommand;
        int rightCommand = distanceCommand + angleCommand;

        this->leftMotor->run(leftCommand);
        this->rightMotor->run(rightCommand);       
    }
}
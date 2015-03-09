#ifndef ENSLAVEMENT
#define ENSLAVEMENT

#include <Arduino.h>
#include <math.h>
#include "Motor.h"
#include "Encoder.h"
#include "Odometry.h"
#include "PID.h"


class Enslavement
{
    public:

        /**
         * @brief Constructor
         */
        
        Enslavement(unsigned long deltaT, double acceleration, double velocityMax,
            Motor *leftMotor, Motor *rightMotor);

        /**
         * @brief Destructor
         */

        ~Enslavement();


        void goTo(CarthesianCoordinates coordinates);

        void turn(double theta);

        void compute();
        
        void setDeltaT(int deltaT);

    private:

        Odometry *odometry;
        Motor *leftMotor;
        Motor *rightMotor;
        Pid *distancePID;
        Pid *anglePID;


        /*
            Constants
         */

        unsigned long deltaT;
        double orientationAcceleration; //ticks.deltaT^-2
        double distanceAcceleration; //ticks.deltaT^-2
        double orientationVelocityMax; //ticks.deltaT^-1
        double distanceVelocityMax;  //ticks.deltaT^-1
        double orientationGain; //ticks
        double distanceGain; //ticks

        /*
            Variables used to generate the trajectory model
         */
        
        unsigned long lastMillis;
        double orientationObjective;
        double distanceObjective;
        double theoricalOrientation;
        double theoricalDistance;
};
#endif

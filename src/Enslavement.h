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
        double theoricalVelocity;


    private:

        Odometry *odometry;
        Motor *leftMotor;
        Motor *rightMotor;
        Pid *distancePID;


        /*
            Constants
         */

        unsigned long deltaT;
        unsigned long lastMillis;

        double distanceVelocityMax;  //ticks.deltaT^-1
        double distanceAcceleration; //ticks.deltaT^-2

        double actualDistanceVelocity;
        double actualDistance;
        double previousDistance;

        double distanceObjective;
        double velocityObjective;

        double theoricalDistance;
};

#endif

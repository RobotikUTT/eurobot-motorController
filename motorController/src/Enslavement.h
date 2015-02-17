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

        double distanceAcceleration; //acceleration in m.s^-2
        double distanceVelocityMax;  //velocity max in m.s^-1
        double orientationAcceleration;    //acceleration in rad.s^-2
        double orientationVelocityMax;     //velocity max in rad.s^-1

        double velocityGain; //acceleration in cm.deltaT^-2

        double distanceObjective;
        double theoricalDistance;
        double currentDistance;
        double lastDistance;
        double distanceCommand;

        double orientationObjective;
        double theoricalOrientation;
        double currentOrientation;
        double lastOrientation;
        double orientationCommand;

        unsigned long deltaT;
        unsigned long lastMillis;

        Motor *leftMotor;
        Motor *rightMotor;
        Odometry *odometry;
        Pid *distancePID;
        Pid *anglePID;
};
#endif

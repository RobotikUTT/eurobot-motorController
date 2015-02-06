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
        
        Enslavement(unsigned long deltaT, int accelerationMax, int velocityMax,
            Motor *leftMotor, Motor *rightMotor);

        /**
         * @brief Destructor
         */

        ~Enslavement();


        void goTo(CarthesianCoordinates coordinates);

        void turn(int theta);

        void compute();
        

    private:

        double acceleration; //acceleration in m.s^-2
        double velocityMax;  //velocity max in m.s^-1

        double velocityGain; //acceleration in cm.deltaT^-2
        double accelerationTime;

        double distanceObjective;
        double currentDistance;
        int distanceCommand;

        double angleObjective;
        double currentAngle;
        int angleCommand;

        unsigned long deltaT;
        unsigned long lastMillis;

        Motor *leftMotor;
        Motor *rightMotor;
        Odometry *odometry;
        PID *distancePID;
        PID *anglePID;
};
#endif

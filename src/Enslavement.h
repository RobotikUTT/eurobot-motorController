#ifndef ENSLAVEMENT
#define ENSLAVEMENT

#include <Arduino.h>
#include <math.h>
#include "Motor.h"
#include "Encoder.h"
#include "Odometry.h"
#include "PID.h"


/**
 * Speed and position control.
 * Provide an API to move the robot at a given (x,y) point
 * without damaging motors by generating a trapezoidal speed
 * profile.
 */

class Enslavement
{
    public:

        static Enslavement* getInst(unsigned long deltaT, double acceleration, double velocityMax,
            Motor *leftMotor, Motor *rightMotor);

        static Enslavement* getInst();

        /**
         * Set destination to a (x, y) point
         * @param coordinates Destination point
         */
        void goTo(double x, double y, bool forceFace);


        /**
         * Set orientation objective
         * @param theta Angle in rad
         */
        void turn(double theta);


        /**
         * Function called periodically (each deltaT).:
         * Generate a trajectory and control motor speed to reach it.
         * Use PID controller in order to deal with errors.
         */
        void compute();

        /**
         * deltaT setter
         * @param deltaT deltaT
         */
        void setDeltaT(int deltaT);

        double theoricalDistanceVelocity;
        double theoricalOrientationVelocity;


    private:

        /**
         * Constructor
         * @param deltaT       Minimal time before compute() call (10^-3s)
         * @param acceleration Maximal acceleration (m.s^-2)
         * @param velocityMax  Maximal velocity (m.s^-1)
         * @param leftEncoder  Left encoder
         * @param rightEncoder Right encoder
         * @param leftMotor    Left motor
         * @param rightMotor   Right motor
         */
        Enslavement(unsigned long deltaT, double acceleration, double velocityMax,
            Motor *leftMotor, Motor *rightMotor);


        /**
         * Destructor
         */
        ~Enslavement();

        static Enslavement *inst;

        Odometry *odometry;
        Motor *leftMotor;
        Motor *rightMotor;
        Pid *distancePID;
        Pid *orientationPID;

        /*
            Constants
         */
        unsigned long deltaT;
        unsigned long lastMillis;

        double distanceVelocityMax;  //ticks.deltaT^-1
        double distanceAcceleration; //ticks.deltaT^-2
        double orientationVelocityMax; //ticks.deltaT^-1
        double orientationAcceleration; //ticks.deltaT^-2

        /*
            Values used to generate objectives and update
            theorical values
         */
        double previousDistance;
        double previousOrientation;

        /*
            Objectives
         */
        double distanceObjective;
        double distanceVelocityObjective;
        double orientationObjective;
        double orientationVelocityObjective;

        /*
            Theorical values
         */
        double theoricalDistance;
        double theoricalOrientation;
}; //5=105mm

#endif

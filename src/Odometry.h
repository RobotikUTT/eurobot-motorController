#ifndef ODOMETRY
#define ODOMETRY

#include <Arduino.h>
#include <math.h>
#include <stdio.h>
#include "Encoder.h"
#include "Motor.h"


typedef struct
{
    double x;
    double y;
} CarthesianCoordinates;


typedef struct
{
    double left;
    double right;
} Ticks;


/**
 * Singleton.
 * Contain all the informations concerning the
 * physical state of the robot. (Position, orientation)
 * Update these informations each time update() is called
 * by retreiving informations of encoders.
 * TODO: find better var-types. (It's all in double for dev
 * convenience)
 */
class Odometry
{
    public:
        static double ENTRAXE; //m

        /**
         * inst getter.
         * TODO: Needs to be refactored
         * @param  leftEncoder  Left encoder
         * @param  rightEncoder Right encoder
         * @return              Odometry unique instance
         */
        static Odometry* getInst(Encoder *leftEncoder, Encoder *rightEncoder);

        /**
         * inst getter
         * @return Odometry unique instance
         */
        static Odometry* getInst();

        /**
         * Convert meters to ticks
         * @param  meters Meters
         * @return        Ticks
         */
        static double metersToTicks(double meters);

        /**
         * Convert ticks to meters
         * @param  ticks Ticks
         * @return       Meters
         */
        static double ticksToMeters(double ticks);

        /**
         * Reset actual position, orientation and
         * Encoders ticks
         */
        void reset();

        /**
         * Read encoders in order to determine new
         * position/orientation
         */
        void update();

        /**
         * leftEncoder getter
         * @return leftEncoder
         */
        Encoder* getLeftEncoder();

        /**
         * leftEncoder setter
         * @param newLeftEncoder new leftEncoder
         */
        void setLeftEncoder(Encoder* newLeftEncoder);


        /**
         * rightEncoder getter
         * @return rightEncoder
         */
        Encoder* getRightEncoder();


        /**
         * rightEncoder setter
         * @param newRightEncoder new rightEncoder
         */
        void setRightEncoder(Encoder* newRightEncoder);


        /**
         * coordinates getter
         * @return coordinates ( (x, y) in m )
         */
        CarthesianCoordinates getCoordinates();


        /**
         * Set new coordinates
         * @param x x value (m)
         * @param y y value (m)
         */
        void setCoordinates(double x, double y);


        /**
         * Orientation getter
         * @return orientation
         */
        double getOrientation();


        /**
         * Orientation setter
         * @param newOrdientation new orientation
         */
        void setOrientation(double newOrdientation);

        Ticks getTicks();

    private:

        /**
         * Constructor
         * @param leftEncoder  left Encoder
         * @param rightEncoder right Encoder
         */
        Odometry(Encoder *leftEncoder, Encoder *rightEncoder);

        /**
         * Destructor
         */
        ~Odometry();

        static Odometry *inst;

        Encoder *leftEncoder;
        Encoder *rightEncoder;

        CarthesianCoordinates coordinates;
        Ticks ticks;
        double orientation;
};
#endif
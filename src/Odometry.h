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


class Odometry
{
    public:
        static const double ENTRAXE;

        static Odometry* getInst(Encoder *leftEncoder, Encoder *rightEncoder);
        
        static double metersToTicks(double meters);
        static double ticksToMeters(double ticks);


        Encoder* getLeftEncoder();
        void setLeftEncoder(Encoder* newLeftEncoder);
        Encoder* getRightEncoder();
        void setRightEncoder(Encoder* newRightEncoder);

        CarthesianCoordinates getCoordinates();
        void setCoordinates(double x, double y);

        double getOrientation();
        void setOrientation(double newOrdientation);

        void reset();
        void update();


    private:
        /**
         * @brief Constructor
         */
        
        Odometry(Encoder *leftEncoder, Encoder *rightEncoder);

        /**
         * @brief Destructor
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

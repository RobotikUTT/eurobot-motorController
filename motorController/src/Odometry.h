#ifndef ODOMETRY
#define ODOMETRY

#include <Arduino.h>
#include <math.h>
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

        static Odometry* getInst(Encoder *leftEncoder, Encoder *rightEncoder);
        static double metersToTicks(double meters);
        static double ticksToMeters(double ticks);

        static const double ENTRAXE;

        CarthesianCoordinates getCoordinates();
        void setCoordinates(CarthesianCoordinates newCoordinates);

        double getOrientation();
        void setOrientation(double newOrdientation);

        Ticks getTicks();
        void setTicks(Ticks newTicks);

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

        void checkOrientation();

        static Odometry *inst;

        Encoder *leftEncoder;
        Encoder *rightEncoder;

        CarthesianCoordinates coordinates;
        double orientation;
        Ticks ticks;
};
#endif

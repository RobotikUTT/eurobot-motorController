#ifndef ODOMETRY
#define ODOMETRY

#include <Arduino.h>
#include <math.h>
#include "Encoder.h"


typedef struct 
{
    double x;
    double y;
} CarthesianCoordinates;


typedef struct 
{
    double rho;
    double theta;
} PolarCoordinates;


typedef struct 
{
    long left;
    long right;   
} Ticks;

class Odometry
{
    public:

        static Odometry* getInst();
        
        static double metersToTicks(double meters);

        static double ticksToMeters(double ticks);

        void reset();

        CarthesianCoordinates getCarthesianCoordinates();

        PolarCoordinates getPolarCoordinates();

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

        static Odometry *inst = NULL;

        Encoder *leftEncoder;
        Encoder *rightEncoder;

        Ticks ticks;
};
#endif

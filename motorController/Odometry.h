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
    double rho;
    double theta;
} PolarCoordinates;


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

        void reset();

        CarthesianCoordinates getCarthesianCoordinates();

        PolarCoordinates getPolarCoordinates();

        Ticks getTicks();

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

        Ticks ticks;
};
#endif

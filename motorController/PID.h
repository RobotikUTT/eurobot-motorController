#ifndef PID
#define PID

#include <Arduino.h>


class PID
{
    public:

        /**
         * @brief Constructor
         * 
         * @param kp proportional gain
         * @param ki integral gain
         * @param kd derivative gain
         * @param deltaT sample time
         * @param input value to compare to setPoint
         * @param setPoint reference
         */
        
        PID(double kp, double ki, double kd, unsigned long deltaT, double *input,
            double *setPoint, double *output);

        /**
         * @brief Destructor
         */

        ~PID();
        
        void compute();


    private:

        double kp;
        double ki;
        double kd;

        unsigned long deltaT;
        unsigned long lastMillis;

        double ITerm;
        double lastInput;

        double *input;
        double *setPoint;
        double *output;
};
#endif

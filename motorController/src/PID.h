#ifndef PID
#define PID

#include <Arduino.h>


class Pid
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
        
        Pid(double kp, double ki, double kd, unsigned long deltaT, double *input,
            double *setPoint, double *output);

        /**
         * @brief Destructor
         */

        ~Pid();
        
        double getKp();
        void setKp(double kp);
        double getKi();
        void setKi(double ki);
        double getKd();
        void setKd(double kd);
        unsigned long getDeltaT();
        void setDeltaT(unsigned long deltaT);
        double* getInput();
        void setInput(double *input);
        double* getOutput();
        void setOutput(double *output);
        double* getSetpoint();
        void setSetpoint(double *setpoint);
        
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

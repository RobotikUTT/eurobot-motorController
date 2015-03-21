#ifndef PID
#define PID

#include <Arduino.h>


/**
 * PID controller used to compute a new motor
 * command from an error between a velocity objective
 * and the real velocity.
 */
class Pid
{
    public:

        /**
         * Constructor
         * @param kp     Proportionnal term
         * @param ki     Integral term
         * @param kd     Derivative term
         * @param deltaT Period
         */
        Pid(double kp, double ki, double kd, unsigned long deltaT);

        /**
         * Destructor
         */
        ~Pid();

        /**
         * Compute a new command
         * @param  input    Real value
         * @param  setPoint Objective
         * @return          Motor command
         */
        double compute(double input, double setPoint);

        /**
         * kp getter
         * @return kp
         */
        double getKp();

        /**
         * kp setter
         * @param kp new kp
         */
        void setKp(double kp);

        /**
         * ki getter
         * @return ki
         */
        double getKi();

        /**
         * ki setter
         * @param ki new ki
         */
        void setKi(double ki);

        /**
         * kd getter
         * @return kd
         */
        double getKd();

        /**
         * kd setter
         * @param kd new kd
         */
        void setKd(double kd);

        /**
         * deltaT getter
         * @return deltaT
         */
        unsigned long getDeltaT();

        /**
         * deltaT setter
         * @param deltaT new deltaT
         */
        void setDeltaT(unsigned long deltaT);


    private:

        double kp;
        double ki;
        double kd;

        unsigned long deltaT;
        unsigned long lastMillis;

        double ITerm;
        double lastInput;
};
#endif
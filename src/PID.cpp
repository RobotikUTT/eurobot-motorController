#include "PID.h"


Pid::Pid(double kp, double ki, double kd, unsigned long deltaT)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;

    this->deltaT = deltaT;
    this->lastMillis = 0;

    this->ITerm = 0;
    this->lastErr = 0;
}


Pid::~Pid()
{

}


double Pid::getKp()
{
    return this->kp;
}


void Pid::setKp(double kp)
{
    this->kp = kp;
}


double Pid::getKi()
{
    return this->ki;
}


void Pid::setKi(double ki)
{
    this->ki = ki * ((double) this->deltaT / 1000);
}


double Pid::getKd()
{
    return this->kd;
}


void Pid::setKd(double kd)
{
    this->kd = kd / ((double) this->deltaT / 1000);
}


unsigned long Pid::getDeltaT()
{
    return this->deltaT;
}


void Pid::setDeltaT(unsigned long deltaT)
{
    if (deltaT > 0)
    {
        double ratio = (double) deltaT / (double) this->deltaT;

        this->ki *= ratio;
        this->kd /= ratio;

        this->deltaT = deltaT;
    }
}


double Pid::compute(double error)
{
    this->ITerm += (this->ki * error);

    double dErr = (error - this->lastErr);

    int pwm = this->kp * error + this->ITerm + this->kd * dErr;

    this->lastErr = error;
    return pwm;
}
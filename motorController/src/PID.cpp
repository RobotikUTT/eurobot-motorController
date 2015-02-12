#include "PID.h"


Pid::Pid(double kp, double ki, double kd, unsigned long deltaT, double *input,
            double *setPoint, double *output)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;

    this->deltaT = deltaT;
    this->lastMillis = 0;

    this->ITerm = 0;
    this->lastInput = 0;

    this->input = input;
    this->setPoint = setPoint;
    this->output = output;
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


double* Pid::getInput()
{
    return this->input;
}


void Pid::setInput(double *input)
{
    this->input = input;
}


double* Pid::getOutput()
{
    return this->output;
}


void Pid::setOutput(double *output)
{
    this->output = output;
}


double* Pid::getSetpoint()
{
    return this->setPoint;
}


void Pid::setSetpoint(double *setPoint)
{
    this->setPoint = setPoint;
}


void Pid::compute()
{
    unsigned long now = millis();
    int timeElapsed = (now - this->lastMillis);

    if (timeElapsed >= this->deltaT)
    {
        //Compute must be called periodically
        
        double error = *(this->setPoint) - *(this->input);
        this->ITerm += (this->ki * error);

        double dInput = (*(this->input) - this->lastInput);

        *(this->output) = this->kp * error + this->ITerm
            - this->kd * dInput;

        this->lastInput = *(input);
        this->lastMillis = now;
    }
}
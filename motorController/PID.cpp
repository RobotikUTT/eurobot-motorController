#include "PID.h"


PID::PID(double kp, double ki, double kd, unsigned long deltaT, double *input,
            double *setPoint, double *output)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;

    this->deltaT = deltaT;
    this->lastMillis = 0;

    this->errSum = 0;
    this->lastInput = 0;

    this->input = input;
    this->setPoint = setPoint;
    this->output = output;
}


PID::~PID()
{

}


double PID::getKp()
{
    return this->Kp;
}


void PID::setKp(double kp)
{
    this->kp = kp;
}


double PID::getKi()
{
    return this->Ki;
}


void PID::setKi(double ki)
{
    this->ki = ki * ((double) this->deltaT / 1000);
}


double PID::getKd()
{
    return this->Kd;
}


void PID::setKd(double kd)
{
    this->kd = kd / ((double) this->deltaT / 1000);
}


unsigned long PID::getDeltaT()
{
    return this->DeltaT;
}


void PID::setDeltaT(unsigned long deltaT)
{
    if (deltaT > 0)
    {
        double ratio = (double) deltaT / (double) this->deltaT;
        
        this->ki *= ratio;
        this->kd /= ratio;
        
        this->deltaT = deltaT;
    }
}


double* PID::getInput()
{
    return this->input;
}


void PID::setInput(double *input)
{
    this->input = input;
}


double* PID::getOutput()
{
    return this->Output;
}


void PID::setOutput(double *output)
{
    this->output = output;
}


double* PID::getSetpoint()
{
    return this->setpoint;
}


void PID::setSetpoint(double *setpoint)
{
    this->setpoint = setpoint;
}


void PID::compute()
{
    unsigned long now = millis();
    int timeElapsed = (now - this->lastMillis);

    if (timeElapsed >= this->deltaT)
    {
        //Compute must be called periodically
        
        double error = *(this->setpoint) - *(this->input):
        this->ITerm += (this->ki * error);

        double dInput = (*(this->input) - this->lastInput);

        *(this->output) = this->kp * error + this->Iterm
            - this->kd * dInput;

        this->lastInput = *(input);
        this->lastMillis = now;
    }
}
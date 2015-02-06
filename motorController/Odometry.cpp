#include "Odometry.h"


Odometry::Odometry(Encoder *leftEncoder, Encoder *rightEncoder) 
{
    this->leftEncoder = leftEncoder;
    this->rightEncoder = rightEncoder;

    this->reset();
}


Odometry::~Odometry()
{

}


Odometry* Odometry::getInst()
{
    if (Odometry::inst == NULL)
    {
        Odometry::inst = new Odometry();
        return Odometry::inst;
    }

    return Odometry::inst;
}


double Odometry::metersToTicks(double meters)
{
    return (meters / 2 / 3.14 / Motor::WHEEL_RADIUS * Encoder::TICK_PER_SPIN);
}


double Odometry::ticksToMeters(double ticks)
{
    return (1/this->metersToticks(ticks));
}


void Odometry::reset()
{
    this->leftTicks = 0;
    this->rightTicks = 0;

    this->ticks = {0, 0};
}


void Odometry::update()
{
    this->ticks.left += this->leftEncoder->getTicks();
    this->ticks.right += this->rightEncoder->getTicks();
    
    this->leftEncoder->resetTicks();
    this->rightEncoder->resetTicks();

}


Ticks Odometry::getTicks()
{
    return this->ticks;
}


PolarCoordinates Odometry::getPolarCoordinates()
{
    double rho = (this->leftTicks + this->rightTicks) / 2; //in encoder ticks
    double theta = (this->leftTicks - this->rightTicks) % 3.14;

    return {Odometry::ticksToMeters(rho), Odometry::ticksToMeters(theta)};
}


CarthesianCoordinates Odometry::getCarthesianCoordinates()
{
    PolarCoordinates polarCoordinates = this->getPolarCoordinates();

    double x = polarCoordinates.rho * cos(polarCoordinates.theta);
    double y = polarCoordinates.rho * sin(polarCoordinates.theta);

    return {Odometry::ticksToMeters(x), Odometry::ticksToMeters(y)};
}
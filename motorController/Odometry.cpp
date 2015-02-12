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


Odometry* Odometry::inst = NULL;


Odometry* Odometry::getInst(Encoder *leftEncoder, Encoder *rightEncoder)
{
    if (Odometry::inst == NULL)
    {
        Odometry::inst = new Odometry(leftEncoder, rightEncoder);
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
    return (ticks * 2 * 3.14 * Motor::WHEEL_RADIUS / Encoder::TICK_PER_SPIN);
}


void Odometry::reset()
{
    this->leftEncoder->resetTicks();
    this->rightEncoder->resetTicks();
    this->ticks.left = 0;
    this->ticks.right = 0;
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
    double rho = (this->ticks.left + this->ticks.right) / 2; //in encoder ticks
    double theta = (long) (this->ticks.left - this->ticks.right) * 100 % (long) 314;

    return {Odometry::ticksToMeters(rho), Odometry::ticksToMeters(theta)};
}


CarthesianCoordinates Odometry::getCarthesianCoordinates()
{
    PolarCoordinates polarCoordinates = this->getPolarCoordinates();

    double x = polarCoordinates.rho * cos(polarCoordinates.theta);
    double y = polarCoordinates.rho * sin(polarCoordinates.theta);

    return {Odometry::ticksToMeters(x), Odometry::ticksToMeters(y)};
}
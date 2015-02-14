#include "Odometry.h"


Odometry::Odometry(Encoder *leftEncoder, Encoder *rightEncoder) 
{
    this->leftEncoder = leftEncoder;
    this->rightEncoder = rightEncoder;

    this->coordinates = {0, 0};
    this->orientation = 0;
    this->reset();
}


Odometry::~Odometry()
{

}


const double Odometry::ENTRAXE = Odometry::metersToTicks(0.5);
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


CarthesianCoordinates Odometry::getCoordinates()
{
    return this->coordinates;
}


void Odometry::setCoordinates(CarthesianCoordinates newCoordinates)
{
    this->coordinates = newCoordinates;
}


double Odometry::getOrientation()
{
    return this->orientation;
}


void Odometry::setOrientation(double newOrientation)
{
    this->orientation = newOrientation;
}


Ticks Odometry::getTicks()
{
    return this->ticks;
}


void Odometry::setTicks(Ticks newTicks)
{
    this->ticks = newTicks;
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
    this->ticks.left = this->leftEncoder->getTicks();
    this->ticks.right = this->rightEncoder->getTicks();
    
    this->leftEncoder->resetTicks();
    this->rightEncoder->resetTicks();
    
    double previousOrientation = this->orientation;
    double radius = Odometry::ENTRAXE * ((1 / 2) + (this->ticks.left / (this->ticks.right - this->ticks.left)));
    
    this->setOrientation(this->orientation + (Odometry::ticksToMeters(this->ticks.right) - Odometry::ticksToMeters(this->ticks.left))/ENTRAXE);
    this->setCoordinates({ this->coordinates.x + radius * (cos(previousOrientation) - cos(this->orientation)), 
        this->coordinates.y + radius * (sin(previousOrientation) - sin(this->orientation)) });
}
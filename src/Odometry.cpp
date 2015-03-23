#include "Odometry.h"


const double Odometry::ENTRAXE = 1; //m
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

    this->coordinates.x = 0;
    this->coordinates.y = 0;

    this->orientation = 0;
}


void Odometry::update()
{
    this->ticks.left = this->leftEncoder->getTicks();
    this->ticks.right = this->rightEncoder->getTicks();

    /* Debug rotary encoders
        Serial.print("left ticks: ");
        Serial.println(ticks.left);
        Serial.print("right ticks: ")
        Serial.println(ticks.right);
    */

    double oldOrientation = this->orientation;
    double rayon = 0;

    double left = Odometry::ticksToMeters(this->ticks.left);
    double right = Odometry::ticksToMeters(this->ticks.right);

    if (((right > 0) && (left < 0)) || ((left > 0) && (right < 0)))
    {
        rayon = Odometry::ENTRAXE * (0.5 - (fmin(fabs(left), fabs(right))/fabs(left - right)));
    }
    else if (right != left)
    {
        rayon = Odometry::ENTRAXE * (0.5 + (fmin(fabs(left), fabs(right)) / fabs(right - left)));
    }

    this->setOrientation(this->orientation + (right - left)/ Odometry::ENTRAXE);

    if (right != left)
    {
        if (fabs(right) > fabs(left))
        {
            this->setCoordinates(this->coordinates.x + rayon * (sin(this->orientation) - sin(oldOrientation)),
            this->coordinates.y + rayon * (cos(oldOrientation) - cos(this->orientation)));
        } else
        {
            this->setCoordinates(this->coordinates.x + rayon * (sin(oldOrientation) - sin(this->orientation)),
            this->coordinates.y + rayon * (cos(this->orientation) - cos(oldOrientation)));
        }
    }
    else
    {
        this->setCoordinates(this->coordinates.x + right * cos(this->orientation),
            this->coordinates.y + right * sin(this->orientation));
    }
}


Encoder* Odometry::getLeftEncoder()
{
    return this->leftEncoder;
}


void Odometry::setLeftEncoder(Encoder* newLeftEncoder)
{
    this->leftEncoder = newLeftEncoder;
}


Encoder* Odometry::getRightEncoder()
{
    return this->rightEncoder;
}


void Odometry::setRightEncoder(Encoder* newRightEncoder)
{
    this->rightEncoder = newRightEncoder;
}


CarthesianCoordinates Odometry::getCoordinates()
{
    return this->coordinates;
}


void Odometry::setCoordinates(double x, double y)
{
    this->coordinates.x = x;
    this->coordinates.y = y;
}


double Odometry::getOrientation()
{
    return this->orientation;
}


void Odometry::setOrientation(double newOrientation)
{
    this->orientation = newOrientation;

    this->orientation = fmod(this->orientation, M_PI * 2);
    if (this->orientation < 0)
        this->orientation += M_PI * 2;
}


Ticks Odometry::getTicks()
{
    return this->ticks;
}


Odometry::Odometry(Encoder *leftEncoder, Encoder *rightEncoder)
{
    this->leftEncoder = leftEncoder;
    this->rightEncoder = rightEncoder;

    this->reset();
}


Odometry::~Odometry(){ }
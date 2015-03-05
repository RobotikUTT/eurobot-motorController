#include "Odometry.h"
 
 
Odometry::Odometry(Encoder *leftEncoder, Encoder *rightEncoder)
{
    this->leftEncoder = leftEncoder;
    this->rightEncoder = rightEncoder;
 
    this->coordinates = (CarthesianCoordinates) {0, 0};
    this->orientation = 0;
    this->reset();
}
 
 
Odometry::~Odometry()
{
 
}
 
 
const double Odometry::ENTRAXE = 1;
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
    this->orientation = fmod(this->orientation, M_PI * 2);
    if (this->orientation < 0)
        this->orientation += M_PI * 2;
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
   
    double oldOrientation = this->orientation;
    double rayon;
 
    double left = Odometry::ticksToMeters(this->ticks.left);
    double right = Odometry::ticksToMeters(this->ticks.right);
 
 
    if (((right > 0) && (left < 0)) || ((left < 0) && (right > 0)))
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
       
        if (right > left)
        {
            this->setCoordinates((CarthesianCoordinates) {this->coordinates.x + rayon * (sin(this->orientation) - sin(oldOrientation)),
            this->coordinates.y + rayon * (cos(oldOrientation) - cos(this->orientation))});
        } else
        {
            this->setCoordinates((CarthesianCoordinates) {this->coordinates.x + rayon * (sin(oldOrientation) - sin(this->orientation)),
            this->coordinates.y + rayon * (cos(this->orientation) - cos(oldOrientation))});
        }
    }
    else
    {
        this->setCoordinates((CarthesianCoordinates) {this->coordinates.x + right * cos(this->orientation),
            this->coordinates.y + right * sin(this->orientation)});
    }
}
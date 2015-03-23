#include "Motor.h"


const double Motor::WHEEL_RADIUS = 0.03;


Motor::Motor(byte pwmPin, byte dirPinA, byte dirPinB)
{
    this->pwmPin = pwmPin;
    this->dirPinA = dirPinA;
    this->dirPinB = dirPinB;
    this->PWM = 0;

    pinMode(pwmPin, OUTPUT);
    pinMode(dirPinA, OUTPUT);
    pinMode(dirPinB, OUTPUT);
}


Motor::~Motor() {};


void Motor::run(int PWM_)
{
    if (PWM_ > 0)
    {
        this->setDir(FORWARD);
    }
    else
    {
        this->setDir(BACKWARD);
    }

    PWM_ = abs(PWM_);
    if (PWM_> 255)
    {
        this->PWM = 255;
    }
    else
    {
        this->PWM = PWM_;
    }

    analogWrite(this->pwmPin, this->PWM);
}


void Motor::stop()
{
    this->PWM = 0;
    digitalWrite(this->dirPinA, HIGH);
    digitalWrite(this->dirPinB, HIGH);
}


void Motor::setDir(Direction dir)
{
    if (dir == BACKWARD)
    {
        this->dir = BACKWARD;
        digitalWrite(this->dirPinA, LOW);
        digitalWrite(this->dirPinB, HIGH);
    }
    else
    {
        this->dir = FORWARD;
        digitalWrite(this->dirPinA, HIGH);
        digitalWrite(this->dirPinB, LOW);
    }
}


int Motor::getPWM()
{
    int sign = (this->dir == BACKWARD) ? -1 : 1;
    return sign * this->PWM;
}
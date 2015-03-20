#include "Motor.h"


/**
 * @brief Constructor
 *
 * @param pwmPin PWM pin
 * @param dir dir pin
 * @param brake brake pin
 */

Motor::Motor(const byte &pwmPin, const byte &dirPinA, const byte &dirPinB)
{
    this->pwmPin = pwmPin;
    this->dirPinA = dirPinA;
    this->dirPinB = dirPinB;
    this->PWM = 0;

    pinMode(pwmPin, OUTPUT);
    pinMode(dirPinA, OUTPUT);
    pinMode(dirPinB, OUTPUT);
}


/**
 * @brief Destructor
 */

Motor::~Motor() {};


const byte Motor::REDUCTOR_RATIO = 48;
const double Motor::WHEEL_RADIUS = 0.03;


/**
 * @brief Set the motor speed
 *
 * @param PWM PWM
 */

void Motor::run(int PWM_)
{
    if (PWM_ > 0)
    {
        this->setDir(0);
    }
    else
    {
        this->setDir(1);
    }

    PWM_ = abs(PWM_);

    if (PWM_ > 255)
    {
        this->PWM = 255;
    }
    else
    {
        this->PWM = PWM_;
    }

    analogWrite(this->pwmPin, this->PWM);
}


/**
 * @brief Stop the motor
 */

void Motor::stop()
{
    this->PWM = 0;
    digitalWrite(this->dirPinA, HIGH);
    digitalWrite(this->dirPinB, HIGH);
}


/**
 * @brief Set the motor dir
 */

void Motor::setDir(const bool &dir)
{
    if (!dir)
    {
        digitalWrite(this->dirPinA, LOW);
        digitalWrite(this->dirPinB, HIGH);
    }
    else
    {
        digitalWrite(this->dirPinA, HIGH);
        digitalWrite(this->dirPinB, LOW);
    }
}


byte Motor::getPWM()
{
    int sign = (this->dir) ? -1 : 1;
    return sign * this->PWM;
}

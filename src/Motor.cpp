#include "Motor.h"


/**
 * @brief Constructor
 *
 * @param pwmPIN PWM pin
 * @param dir dir pin
 * @param brake brake pin
 */

Motor::Motor(const byte &pwmPIN, const byte &dirPIN, const byte &brake)
{
    this->pwmPIN = pwmPIN;
    this->dirPIN = dirPIN;
    this->brake = brake;
    this->dir = 0;
    this->PWM = 0;

    pinMode(pwmPIN, OUTPUT);
    pinMode(dir, OUTPUT);
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

    analogWrite(this->pwmPIN, this->PWM);
}


/**
 * @brief Stop the motor
 */

void Motor::stop()
{
    this->PWM = 0;
    analogWrite(this->pwmPIN, this->PWM);
}


/**
 * @brief Set the motor dir
 */

void Motor::setDir(const bool &dir)
{
    if (this->dir != dir)
    {
        this->dir = dir;
        digitalWrite(this->dir, dir);
    }
}


byte Motor::getPWM()
{
    int sign = (this->dir) ? -1 : 1;
    return sign * this->PWM;
}

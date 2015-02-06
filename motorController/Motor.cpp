#include "Motor.h";


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


/**
 * @brief Set the motor speed
 * 
 * @param PWM PWM
 */

void Motor::run(const int &PWM)
{
    this->PWM = abs(PWM);
    
    if (this->PWM > 255)
        this->PWM = 255;

    if (PWM > 0)
    {
        this->setDir(0);
    }
    else
    {
        this->setDir(1);
    }

    analogWrite(this->pwmPIN, PWM);
}


/**
 * @brief Stop the motor
 */

void Motor::stop()
{
    this->PWM = 0;
    analogWrite(this->pwmPIN, PWM);
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
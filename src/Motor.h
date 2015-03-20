#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h> 


class Motor
{
    public:
        Motor(const byte &pwmPin, const byte &dirPinA, const byte &dirPinB);
        ~Motor();

        void run(int PWM);
        void stop();
        byte getPWM();
        
        static const byte REDUCTOR_RATIO;
        static const double WHEEL_RADIUS;


    protected:
        void setDir(const bool &dir); 

        byte PWM;
        bool dir;
        byte pwmPin;
        byte dirPinA;
        byte dirPinB;
};

#endif
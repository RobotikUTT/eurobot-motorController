#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h> 


class Motor
{
    public:
        Motor(const byte &pwmPIN, const byte &dirPIN, const byte &brake = 0);
        ~Motor();

        void run(const int &PWM);
        void stop();
        
        static const byte REDUCTOR_RATIO  = 48;
        static const double WHEEL_RADIUS = 0.03;


    protected:
        void setDir(const bool &dir); 

        byte PWM;
        bool dir;
        byte pwmPIN;
        byte dirPIN;
        byte brake;
};

#endif
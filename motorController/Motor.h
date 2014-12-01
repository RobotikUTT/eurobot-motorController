#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h> 


class Motor
{
    static const bool FORWARD = 0;
    static const bool BACKWARD = 1;

    public:
        Motor(const byte &pwmPIN, const byte &dirPIN, const byte &brake = 0);
        ~Motor();

        void run(const int &PWM);
        void stop();
        

    protected:
        void setDir(const bool &dir); 

        byte PWM;
        bool dir;
        byte pwmPIN;
        byte dirPIN;
        byte brake;
};

#endif
#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h> 


class Encoder
{
    public:
        Encoder(const byte &chanA, const byte &chanB, const byte &interrupt);
        ~Encoder();

        long getTicks();
        void listenToTicks();


    protected:
        byte chanA;
        byte chanB;
        byte interrupt;
        long tickNumber;
};

#endif
#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h> 


class Encoder
{
    public:
        Encoder(const byte &chanA, const byte &chanB);
        TEST_VIRTUAL ~Encoder();

        TEST_VIRTUAL long getTicks();
        void resetTicks();
        void listenToTicks();

        static const int TICK_PER_SPIN = 500;


    protected:
        byte chanA;
        byte chanB;
        long tickNumber;

};

#endif
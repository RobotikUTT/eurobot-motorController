#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>


/**
 *  Rotary encoder.
 *  Provide a feedback on Motor in order to control it
 *  with the use of Enslavement class.
 *  Channel A is bound and interrupt PIN,
 *  channel B is used to determine if the tick
 *  is clowise or counter-clockwise.
 */
class Encoder
{
    public:

        /**
         * Tick per wheel spin
         */
        static const int TICK_PER_SPIN = 500;

        /**
         * Constructor
         * @param chanA First channel
         * @param chanB Second channel
         */
        Encoder(byte chanA, byte chanB);

        /**
         * Destructor
         * Must be virtual to be mocked
         */
        TEST_VIRTUAL ~Encoder();

        /**
         * ticks getter
         * Mockable method
         * @return ticks
         */
        TEST_VIRTUAL double getTicks();

        /**
         * Reset ticks
         */
        void resetTicks();

        /**
         * Called on interrupt
         * Add tick depending on the value of channel B
         */
        void listenToTicks();


    protected:

        byte chanA;
        byte chanB;
        double ticks;

};

#endif
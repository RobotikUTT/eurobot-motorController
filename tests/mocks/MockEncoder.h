#ifndef MOCKENCODER_CPP
#define MOCKENCODER_CPP

#include "gmock/gmock.h"
#include "Encoder.h"


/**
 * Encoder mock class designed to provide a fake
 * getTicks method in order to test Odometry and Enslavement
 */

class MockEncoder: public Encoder
{
    public:
        MockEncoder(const byte &chanA, const byte &chanB): Encoder(chanA, chanB) {};
        MOCK_METHOD0(getTicks, double());
};

#endif
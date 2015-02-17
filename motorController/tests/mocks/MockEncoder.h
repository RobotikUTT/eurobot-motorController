#ifndef MOCKENCODER_CPP
#define MOCKENCODER_CPP

#include "gmock/gmock.h"
#include "Encoder.h"


class MockEncoder: public Encoder
{   
    public:
        MockEncoder(const byte &chanA, const byte &chanB): Encoder(chanA, chanB) {};
        MOCK_METHOD0(getTicks, long());
};

#endif
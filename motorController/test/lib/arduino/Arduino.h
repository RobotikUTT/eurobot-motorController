#ifndef ARDUINO_H
#define ARDUINO_H

#include <iostream>
#include "stdint.h"


/**
 * Arduino types
 */

typedef uint8_t byte;


/**
 * @brief Serial mock-up
 */

class FakeSerial 
{
    public:
        FakeSerial(byte portNumber);
        ~FakeSerial();

        void begin(unsigned long speed);
        void end();

        size_t write(byte value);


    private:
        byte portNumber;
};

#endif
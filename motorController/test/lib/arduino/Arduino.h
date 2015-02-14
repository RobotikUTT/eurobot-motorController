#ifndef ARDUINO_H
#define ARDUINO_H

#include <iostream>


/**
 * Arduino types
 */

typedef uint8_t byte;


/**
 * @brief Serial mock-up
 */

class Serial 
{
    public:
        Serial(byte portNumber);
        ~Serial();

        void begin(unsigned long speed);
        void end();

        size_t write(byte value);
};




#endif
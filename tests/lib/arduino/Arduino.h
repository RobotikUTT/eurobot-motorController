#ifndef ARDUINO_H
#define ARDUINO_H

#include <cstring>
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <sys/timeb.h>
#include "stdint.h"


/**
 * Macros
 */

// undefine stdlib's abs if encountered
#ifdef abs
    #undef abs
#endif

#define free(x) std::free(x)
#define malloc(x) std::malloc(x)
#define abs(x) ((x) > 0 ? (x) : -(x))
#define M_PI       3.14159265358979323846
 

/**
 * Arduino types
 */

typedef uint8_t byte;


/**
 * Constants
 */

extern const byte INPUT;
extern const byte OUTPUT;
extern const byte HIGH;
extern const byte LOW;


/**
 * Serial emulation
 */

class FakeSerial 
{
    public:
        FakeSerial(byte portNumber);
        ~FakeSerial();

        void begin(unsigned long speed);
        void end();

        int read();
        int peek();
        int available();

        size_t write(byte value);

        size_t write(const char *str);
        size_t write(const byte *buffer, size_t length);

        size_t print(const char *str);
        size_t println(const char *str);
        size_t print(char value);
        size_t println(char value);

        size_t print(int value, int base);
        size_t print(int value);
        size_t println(int value, int base);
        size_t println(int value);

        size_t print(unsigned int value, int base);
        size_t print(unsigned int value);
        size_t println(unsigned int value, int base);
        size_t println(unsigned int value);

        size_t print(long value, int base);
        size_t print(long value);
        size_t println(long value, int base);
        size_t println(long value);

        size_t print(unsigned long value, int base);
        size_t print(unsigned long value);
        size_t println(unsigned long value, int base);
        size_t println(unsigned long value);

        size_t println();


    private:
        byte portNumber;
};


/**
 * Serial objects
 */

extern FakeSerial Serial;
extern FakeSerial Serial1;
extern FakeSerial Serial2;
extern FakeSerial Serial3;


/**
 * Time
 */

unsigned long millis();


/**
 * Digital I/O
 */

void pinMode(byte pin, byte mode);
int digitalRead(byte pin);
void digitalWrite(byte pin, int value);


/**
 * Analog I/O
 */

void analogWrite(byte pin, int value);

#endif
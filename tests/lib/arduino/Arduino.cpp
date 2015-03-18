#include "Arduino.h"


/**
 * Constants
 */

const byte INPUT = 0;
const byte OUTPUT = 1;
const byte HIGH = 1;
const byte LOW = 0;


/**
 * Serial mock-up
 */

FakeSerial::FakeSerial(byte portNumber)
{
    this->portNumber = portNumber;
}


FakeSerial::~FakeSerial()
{

}


void FakeSerial::begin(unsigned long speed) 
{

}


void FakeSerial::end() 
{

}


int FakeSerial::read()
{
    return 1;
}


int FakeSerial::peek()
{
    return 1;
}


int FakeSerial::available()
{
    return 1;
}


size_t FakeSerial::write(byte value) 
{
    return 1;
}


size_t FakeSerial::write(const byte *buffer, size_t length) 
{
    return length;
}


size_t FakeSerial::print(const char *str)
{
    std::cout << str;

    return sizeof(str)/sizeof(char);
}


size_t FakeSerial::println(const char *str)
{
    std::cout << str << "\n";

    return sizeof(str)/sizeof(char);
}


size_t FakeSerial::print(double value)
{
    std::cout << value;

    return sizeof(value);
}


size_t FakeSerial::println(double value)
{
    std::cout << value << "\n";

    return sizeof(value);
}


/**
 * Serial objects
 */

FakeSerial Serial(0), Serial1(1), Serial2(2), Serial3(3);


/**
 * Time
 */

timeb startTime;

unsigned long millis() 
{
  timeb now;
  ftime(&now);

  return (now.time  - startTime.time) * 1000 + (now.millitm - startTime.millitm);
}


/**
 * Digital I/O
 */

void pinMode(byte pin, byte mode) 
{
    // TODO: set the mode of the pin in a Map and check when using digitalRead/write
}


int digitalRead(byte pin)
{
    return 1;
}


void digitalWrite(byte pin, int value)
{

}


/**
 * Analog I/O
 */
 
 void analogWrite(byte pin, int value)
 {

 }
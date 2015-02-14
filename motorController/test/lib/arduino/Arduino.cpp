#include <cstring>
#include <iostream>
#include <iomanip>

#include "Arduino.h"


FakeSerial::FakeSerial(byte portNumber)
{
  this->portNumber = portNumber;
}


FakeSerial::~FakeSerial()
{

}


void FakeSerial::begin(unsigned long speed) {
  return;
}


void FakeSerial::end() {
  return;
}


size_t FakeSerial::write(byte value) {
  return 1;
}


FakeSerial Serial(0);
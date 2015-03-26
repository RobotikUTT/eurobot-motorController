#include "Encoder.h"


Encoder::Encoder(byte chanA, byte chanB)
{
    this->chanA = chanA;
    this->chanB = chanB;

    this->ticks = 0;

    pinMode(chanA, INPUT);
    pinMode(chanB, INPUT);
}


Encoder::~Encoder() {}


double Encoder::getTicks()
{
    return this->ticks;
}


void Encoder::resetTicks()
{
    this->ticks = 0;
}


void Encoder::listenToTicks()
{
    if(digitalRead(this->chanB) == HIGH)
    {
        this->ticks++;
    }
    else
    {
        this->ticks--;
    }
}
#include "Encoder.h"


/**
 * @brief Constructor
 * 
 * @param chanA channel A PIN
 * @param chanB channel B PIN
 * @param interrupt interrupt PIN
 */

Encoder::Encoder(const byte &chanA, const byte &chanB, const byte &interrupt)
{
    this->chanA = chanA;
    this->chanB = chanB;
    this->interrupt = interrupt;

    pinMode(chanA, INPUT);
    pinMode(chanB, INPUT);
}


/**
 * @brief Destructor
 */

Encoder::~Encoder() {}


/**
 * @brief Return tick number 
 * @return tick number
 */

long Encoder::getTicks()
{
    long tickNumber = this->tickNumber;

    return tickNumber;
}


/**
 * @brief Reset tick number
 */

void Encoder::resetTicks()
{
    this->tickNuber = 0;
}


/**
 * @brief Increment or decrement the tick number following
 * a dir convention
 */

void Encoder::listenToTicks()
{
    if(digitalRead(chanB) == HIGH) 
    {
        this->tickNumber++;
    }
    else
    {
        this->tickNumber--;
    }
}
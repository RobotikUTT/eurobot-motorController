#include "Encoder.h"


/**
 * @brief Constructor
 * 
 * @param chanA channel A PIN
 * @param chanB channel B PIN
 * @param interrupt interrupt PIN
 */

Encoder::Encoder(const byte &chanA, const byte &chanB)
{
    this->chanA = chanA;
    this->chanB = chanB;

    this->tickNumber = 0;

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
    return this->tickNumber;
}


/**
 * @brief Reset tick number
 */

void Encoder::resetTicks()
{
    this->tickNumber = 0;
}


/**
 * @brief Increment or decrement the tick number following
 * a dir convention
 */

void Encoder::listenToTicks()
{
    if(digitalRead(this->chanB) == HIGH) 
    {
        this->tickNumber++;
    }
    else
    {
        this->tickNumber--;
    }
}
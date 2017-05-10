#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>

void lireMessage();

/**
 * Dernière commande reçue en série
 */
extern String commande;

#endif

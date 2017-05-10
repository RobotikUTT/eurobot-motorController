#ifndef ASSERVISSEMENT_H
#define ASSERVISSEMENT_H

#include <Arduino.h>
#include "constantes.h"
#include "communication.h"
#include "motionPlanning.h"
#include "digitalPID.h"
#include "moteurs.h"

void step(Moteur* moteur);
void avance(Moteur* moteur, int distance);

/**
 * Contrôle de la boucle d'asservissement
 */
extern boolean asserv;

#endif

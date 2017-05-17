#ifndef ASSERVISSEMENT_H
#define ASSERVISSEMENT_H

#include <Arduino.h>
#include "constantes.h"
#include "motionPlanning.h"
#include "digitalPID.h"
#include "moteurs.h"
#include "communication.h"

void asserv_step(Moteur* moteur);
void asserv_avance(Moteur* moteur, float distance);

/**
 * Contrôle de la boucle d'asservissement
 */

extern boolean asserv;

#endif

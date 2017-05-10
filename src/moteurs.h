#ifndef MOTEURS_H
#define MOTEURS_H

#include "constantes.h"
#include "communication.h"
#include "digitalPID.h"
#include "codeurs.h"

typedef struct {
    uint8_t en;
    uint8_t dir;
    uint8_t brk;
    volatile long* ticks;
    DigitalPID* pid;
} Moteur;

void stop(Moteur* moteur);

extern Moteur moteurG;
extern Moteur moteurD;

#endif

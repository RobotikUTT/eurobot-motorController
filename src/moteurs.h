#ifndef MOTEURS_H
#define MOTEURS_H

#include "constantes.h"
#include "digitalPID.h"
#include "codeurs.h"

typedef struct {
    uint8_t en;
    uint8_t dir;
    uint8_t brk;
    uint8_t avance;
    uint8_t recule;
    volatile long* ticks;
    DigitalPID* pid;
} Moteur;

void moteur_avance(Moteur* moteur, int pwm);
void moteur_recule(Moteur* moteur);
void moteur_stop(Moteur* moteur);
void moteur_test(Moteur* moteur);

extern Moteur moteurG;
extern Moteur moteurD;

#endif

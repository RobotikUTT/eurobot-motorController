#ifndef CODEURS_H
#define CODEURS_H

/**
 * Routines d'interruption codeur
 */

void codeur_G();
void codeur_D();

/**
 * Ticks codeurs. 
 * `volatile` pour indiquer au compilateur
 * que les valeurs sont amenées à être changées 
 * n'importe quand (interruptions).
 * `extern` car ces variables sont globales dans
 * le programme.
 */

extern volatile long ticksG;
extern volatile long ticksD;

#endif

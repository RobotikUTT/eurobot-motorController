#include <Arduino.h>
#include "constantes.h"
#include "digitalPID.h"

void pid_init(DigitalPID* pid) {
    pid->sommeErreur    = 0;
    pid->derniereErreur = 0;
}

void pid_config(DigitalPID* pid, unsigned int dt, double kp, double ki, double kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

int pid_calcul(DigitalPID* pid, int erreur) {
    // Terme integral
    pid->sommeErreur += erreur;

    // Terme dérivé avec anti windup
    unsigned long deriveeErreur = erreur - pid->derniereErreur;

    // Calcul de la sortie
    int sortie = pid->kp * erreur + pid->ki * pid->sommeErreur + pid->kd * deriveeErreur;
    
    // On mémorise la dernière erreur pour le futur calcul de dérivée
    pid->derniereErreur = erreur;

    return sortie;
}

DigitalPID pidG;
DigitalPID pidD;

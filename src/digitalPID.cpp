#include <Arduino.h>
#include "constantes.h"
#include "digitalPID.h"

void pid_init(DigitalPID* pid) {
    pid->q0  = 0;
    pid->q1  = 0;
    pid->q2  = 0;
    pid->em1 = 0;
    pid->em2 = 0;
    pid->y   = 0;
}

void pid_config(DigitalPID* pid, unsigned int dt, double kp, double ki, double kd) {
    pid->q0 = kp*(ki*dt/2+kd/dt+1);
    pid->q1 = kp*(ki*dt/2-kd/dt*2-1);
    pid->q2 = kp*kd/dt;
}

int pid_calcul(DigitalPID* pid, int erreur) {
    pid->y += pid->q0 * erreur + pid->q1 * pid->em1 + pid->q2 * pid->em2;

    return pid->y;
}

DigitalPID pidG;
DigitalPID pidD;

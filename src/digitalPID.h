#ifndef DIGITALPID_H
#define DIGITALPID_H

typedef struct {
    double kp;
    double ki;
    double kd;
    double sommeErreur;
    double derniereErreur;
} DigitalPID;

void pid_init(DigitalPID* pid);
void pid_config(DigitalPID* pid, unsigned int dt, double kp, double ki, double kd);
int pid_calcul(DigitalPID* pid, int erreur);

extern DigitalPID pidG;
extern DigitalPID pidD;

#endif

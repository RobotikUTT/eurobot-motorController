#ifndef DIGITALPID_H
#define DIGITALPID_H

typedef struct {
    float q0;
    float q1;
    float q2;
    int em1;
    int em2;
    int y;
} DigitalPID;

void pid_init(DigitalPID* pid);
void pid_config(DigitalPID* pid, unsigned int dt, double kp, double ki, double kd);
int pid_calcul(DigitalPID* pid, int erreur);

extern DigitalPID pidG;
extern DigitalPID pidD;

#endif

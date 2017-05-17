#ifndef MOTIONPLANNING_H
#define MOTIONPLANNING_H

#include <Arduino.h>
#include "bbpr.h"

void computePeriods(double Xpeak[], double T[]);
double getSetpoint(double Xpeak[], double T[], double t);

#endif

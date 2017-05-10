#include <Arduino.h>
#include "moteurs.h"

void stop(Moteur* moteur) {
    digitalWrite(moteur->en, LOW);
    digitalWrite(moteur->dir, LOW);
    digitalWrite(moteur->brk, LOW);
}

Moteur moteurG = { MOT_G_EN, MOT_G_DIR, MOT_G_BRK, NULL, NULL };
Moteur moteurD = { MOT_D_EN, MOT_D_DIR, MOT_D_BRK, NULL, NULL };

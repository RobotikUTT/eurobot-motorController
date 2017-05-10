#include <Arduino.h>
#include "constantes.h"
#include "codeurs.h"

void codeur_G() {
    if (digitalRead(ENC_G_B) == ENC_G_SIGN){
        ticksG++;
    } else  {
        ticksG--;
    }
}

void codeur_D() {
    if (digitalRead(ENC_D_B) == ENC_D_SIGN){
        ticksD++;
    } else  {
        ticksD--;
    }
}

volatile long ticksG = 0;
volatile long ticksD = 0;

#include <Arduino.h>
#include "moteurs.h"

void moteur_avance(Moteur* moteur, int pwm) {
    if (pwm > 0) {
        digitalWrite(moteur->dir, moteur->avance);
    } else if (pwm < 0) {
        digitalWrite(moteur->dir, moteur->recule);
    } 

    digitalWrite(moteur->brk, LOW);
    analogWrite(moteur->en, abs(pwm));
}

void moteur_stop(Moteur* moteur) {
    digitalWrite(moteur->en, LOW);
    digitalWrite(moteur->dir, LOW);
    digitalWrite(moteur->brk, LOW);
}

void moteur_test(Moteur* moteur) {
    moteur_stop(moteur);

    Serial.println("Avance");
    moteur_avance(moteur, 50); 
    delay(2000);
    moteur_stop(moteur);
    Serial.print("Ticks: ");
    Serial.println(*moteur->ticks);

    Serial.println("Recule");
    moteur_avance(moteur, -50);
    delay(2000);
    moteur_stop(moteur);
    Serial.print("Ticks: ");
    Serial.println(*moteur->ticks);
}

Moteur moteurG = { 
    MOT_G_EN, 
    MOT_G_DIR, 
    MOT_G_BRK,
    MOT_G_AV,
    MOT_G_REC,
    NULL, 
    NULL 
};

Moteur moteurD = { 
    MOT_D_EN, 
    MOT_D_DIR, 
    MOT_D_BRK, 
    MOT_D_AV,
    MOT_D_REC,
    NULL, 
    NULL 
};

#include "asservissement.h"

void step(Moteur* moteur) {
    // On stoppe tout
    stop(moteur);

    // Reset codeur
    moteur->ticks = 0;
    
    unsigned long debut = millis();
    unsigned long avant = debut;

    while(asserv) {
        digitalWrite(moteur->en, HIGH);
        unsigned long maintenant = millis();
        unsigned int diff        = maintenant - avant;

        // On fait l'acquisition à DT régulier
        if (diff >= DT) {
            avant = maintenant;

            // Envoi des données
            Serial.print(maintenant-debut);
            Serial.print(" ");
            Serial.println(ticksG);
        }

        lireMessage();
    }
}

void avance(Moteur* moteur, int distance) {
    Serial.print("Avance: ");
    Serial.println(distance);

    // On stoppe tout
    stop(moteur);

    distance /= PRECISION;

    Serial.print("Ticks: ");
    Serial.println((int)distance);

    double Xpeak[ORDER+1] = { 0, V_MAX, A_MAX };
    double T[ORDER+1]     = { 0, 0, 0 };
    
    Xpeak[0] = (int)distance;
    computePeriods(Xpeak, T);
    Serial.println(V_MAX);
    Serial.println(A_MAX);
    Serial.println(Xpeak[0]);
    Serial.println(Xpeak[1]);
    Serial.println(Xpeak[2]);
    Serial.println(T[0]);
    Serial.println(T[1]);
    Serial.println(T[2]);

    unsigned long debut = millis();
    unsigned long avant = debut;

    while(asserv) {
        digitalWrite(moteur->en, HIGH);
        unsigned long maintenant = millis();
        unsigned int diff        = maintenant - avant;

        // On fait l'acquisition à DT régulier
        if (diff >= DT) {
            avant = maintenant;

            // On calcule l'objectif
            int objectif = getSetpoint(Xpeak, T, (maintenant-debut)/1000.0);
            int erreur   = objectif - *(moteur->ticks);
            int commande = pid_calcul(moteur->pid, erreur);
            
            analogWrite(moteur->en, commande);
            
            // Envoi des données
            Serial.print(maintenant-debut);
            Serial.print(" ");
            Serial.println(*(moteur->ticks));
        }

        lireMessage();
    }
}

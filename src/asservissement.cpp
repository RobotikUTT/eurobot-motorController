#include "asservissement.h"

void asserv_step(Moteur* moteur) {
    // On stoppe tout
    moteur_stop(moteur);

    // Reset codeur
    *moteur->ticks = 0;
    
    unsigned long debut = millis();
    unsigned long avant = debut;

    while(asserv) {
        moteur_avance(moteur, 255);
        unsigned long maintenant = millis();
        unsigned int diff        = maintenant - avant;

        // On fait l'acquisition à DT régulier
        if (diff >= DT) {
            avant = maintenant;

            // Envoi des données
            Serial.print(maintenant-debut);
            Serial.print(" ");
            Serial.println(*moteur->ticks);

            // Reset des ticks, on fait un step de vitesse
            *moteur->ticks = 0;
        }

        lireMessage();
    }
}

void asserv_avance(Moteur* moteur, float distance) {
    // On stoppe tout
    moteur_stop(moteur);

    long ticks = (long)(distance/PRECISION);


    double Xpeak[ORDER+1] = { distance, V_MAX, A_MAX };
    double T[ORDER+1]     = { 0, 0, 0 };

    computePeriods(Xpeak, T);

    unsigned long debut = millis();
    unsigned long avant = debut;

    while(asserv) {
        unsigned long maintenant = millis();
        unsigned int diff        = maintenant - avant;

        // On fait l'acquisition à DT régulier
        if (diff >= DT) {
            avant = maintenant;

            // On calcule l'objectif
            // int objectif = getSetpoint(Xpeak, T, (maintenant-debut)/1000.0);
            int objectif = ticks;
            int erreur   = objectif - *(moteur->ticks);
            int commande = pid_calcul(moteur->pid, erreur);
            
            // Envoi des données
            Serial.print(maintenant-debut);
            Serial.print(" ");
            Serial.print(objectif);
            Serial.print(" ");
            Serial.print(*(moteur->ticks));
            Serial.println();

            moteur_avance(moteur, commande);
        }

        lireMessage();
    }

    moteur_stop(moteur);
}

boolean asserv = false;

#include <Arduino.h>
#include "constantes.h"
#include "codeurs.h"
#include "moteurs.h"
#include "digitalPID.h"
#include "motionPlanning.h"
#include "communication.h"
#include "asservissement.h"

/**
 * Point d'entrée
 */

void setup(){
    // Liaison série
    Serial.begin(115200);
    
    /**
     * Initialisation GPIO
     */
    
    pinMode(MOT_G_BRK, OUTPUT);
    pinMode(MOT_G_EN, OUTPUT);
    pinMode(MOT_G_DIR, OUTPUT);

    pinMode(MOT_D_BRK, OUTPUT);
    pinMode(MOT_D_EN, OUTPUT);
    pinMode(MOT_D_DIR, OUTPUT);

    pinMode(ENC_D_A, INPUT);
    pinMode(ENC_D_B, INPUT);

    pinMode(ENC_G_A, INPUT);
    pinMode(ENC_G_B, INPUT);


    /**
     * Interruptions
     */
    
    attachInterrupt(0, codeur_D, RISING);
    attachInterrupt(1, codeur_G, RISING);   
    
    /**
     * Fréquence de PWM
     */
    
    TCCR1B = (TCCR1B & 0xF8) | 0x01 ;

    /**
     * Désactivation de l'asservissement
     */

    asserv = false;
    
    /**
     * Initialisation des moteurs
     */
    
    moteur_stop(&moteurG);
    moteur_stop(&moteurD);

    moteurG.ticks = &ticksG;
    moteurD.ticks = &ticksD;
    moteurG.pid   = &pidG;
    moteurD.pid   = &pidD;

    /**
     * Initialisation des PID
     */
    
    pid_init(&pidG);
    pid_init(&pidD);    
    pid_config(&pidG, DT, 1, 0, 0);
    pid_config(&pidD, DT, 0, 0, 0);

    /**
     * reset codeurs
     */

    ticksG = 0;
    ticksD = 0; 

    /**
     * Prêt, on arrête tout puis on lance les tests
     */
    
    Serial.println("Setup");
}

void loop() {
    lireMessage();
}

#include "communication.h"

void lireMessage() {
    if (Serial.available()) {
        // Buffer d'entrée
        char buffer_in[MAX_TAILLE + 1];
        byte size = Serial.readBytes(buffer_in, MAX_TAILLE);
        // On ajoute un octet null pour marquer la fin de la String
        buffer_in[size] = 0;

        // On lis chaque argument, séparés par des `:`
        char* args = strtok(buffer_in, ":");

        int commande = atoi(args);
        args = strtok(0, ":");


        switch (commande) {
            case 0x00: {
                Serial.println("test");
                Serial.println("Moteur gauche : ");
                moteur_test(&moteurG);
                Serial.println("Moteur droit : ");
                moteur_test(&moteurD);
                break;
            }

            case 0x01: {
                int cote = atoi(args);
                args = strtok(0, ":");

                asserv = true;
                if (cote) {
                    Serial.println("gauche");
                    asserv_step(&moteurG);
                } else {
                    Serial.println("droit");
                    asserv_step(&moteurD);
                }

                break;
            }

            case 0x02: {
                Serial.print("avance ");
                
                int cote = atoi(args);
                args = strtok(0, ":");

                int dist = atoi(args);
                args = strtok(0, ":");

                asserv = true;
                if (cote) {
                    Serial.print("gauche ");
                    Serial.println(dist);
                    asserv_avance(&moteurG, dist);
                } else {
                    Serial.print("droit ");
                    Serial.println(dist);
                    asserv_avance(&moteurD, dist);
                }

                break;
            }

            case 0x03: {
                Serial.println("stop");
                asserv = false;
                moteur_stop(&moteurG);
                moteur_stop(&moteurD);

                break;
            }

            case 0x04: {
                Serial.print("Reglage PID ");

                int cote = atoi(args);
                args = strtok(0, ":");

                float kp = atof(args);
                args = strtok(0, ":");
                float ki = atof(args);
                args = strtok(0, ":");
                float kd = atof(args);
                args = strtok(0, ":");

                if (cote) {
                    Serial.print("gauche ");
                    pid_config(&pidG, DT, kp, ki, kd);
                } else {
                    Serial.print("droit ");
                    pid_config(&pidD, DT, kp, ki, kd);
                }

                Serial.print("kp ");
                Serial.print(kp, 5);
                Serial.print(" ki ");
                Serial.print(ki, 5);
                Serial.print(" kd ");
                Serial.println(kd, 5);

                break;
            }

            default: {
                Serial.println("Commande inconnue");
            }

        }
    }
}

#include "communication.h"

void lireMessage() {
    if (Serial.available()) {
        commande = Serial.readString();

        if (commande == "stepG\n") {

        } else if (commande == "avanceG\n") {

        } else if (commande == "stop\n") {

        }
    }
}

String commande;
boolean asserv  = false;


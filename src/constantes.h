#ifndef CONSTANTES_H
#define CONSTANTES_H

/**
 * Communication
 */

#define MAX_TAILLE 30 // caractères

/**
 * Mécanique
 */

#define DIAMETRE    52                                  // mm
#define FACTEUR     1
#define RESOLUTION  FACTEUR  * 1024                     
#define PRECISION  (double)((PI*DIAMETRE) / (double)(RESOLUTION)) // mm.tick^-1

/**
 * Asservissement
 */

#define DT          50                                  // ms
#define V_MAX      (int)((750.0 / PRECISION))           // tick.s^-1
#define A_MAX      (int)((500.0 / PRECISION))           // tick.s^-2
#define ORDER      2                                    // commande trapézoïdale

/**
 * A modifier pour changer le sens de comptage des codeurs
 */

#define ENC_D_SIGN LOW
#define ENC_G_SIGN HIGH

/**
 * A modifier pour changer le sens de rotation des moteurs
 */

#define MOT_G_AV  HIGH
#define MOT_G_REC LOW
#define MOT_D_AV  LOW
#define MOT_D_REC HIGH

/**
 * Pins
 */

#define MOT_G_EN  5
#define MOT_G_DIR 9
#define MOT_G_BRK 8

#define MOT_D_EN  5
#define MOT_D_DIR 9
#define MOT_D_BRK 8

#define ENC_D_A   2
#define ENC_D_B   4

#define ENC_G_A   3
#define ENC_G_B   7

#endif

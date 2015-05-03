from math import *
import matplotlib.pyplot as plt

plot = []
plot2 = []
plot3 = []
plot4 = []

vitesse_max = 1
acceleration = 0.01

vitesse_courante = 0
position_courante = 0
position_precedente = 0

position_consigne = 150
vitesse_consigne = 0

vitesse_theorique = 0
position_theorique = 0


def asservissement_position():
    global vitesse_consigne
    distance_restante = position_consigne - position_theorique

    fini = False
    if (fabs(distance_restante)  <= acceleration):
        fini = True

    if (fini):
        print "FINI"
        vitesse_consigne = 0
    else:
        if ((distance_restante <= pow(vitesse_theorique, 2) /( 2 * acceleration))):
            vitesse_consigne = 0
        else:

            if (distance_restante > 0):
                vitesse_consigne = vitesse_max

            elif (distance_restante < 0):
                vitesse_consigne = -vitesse_max


    print distance_restante
    plot2.append(distance_restante)
    plot.append(position_theorique)
    plot3.append(vitesse_theorique)
    plot4.append(vitesse_consigne)

def asservissement_vitesse():
    global vitesse_theorique
    # print "vitesse th " + str(vitesse_theorique)

    if (round(vitesse_theorique, 2) == vitesse_consigne):
        print "FINI2"
        return

    elif (round(vitesse_theorique + acceleration, 2) <= vitesse_consigne):
        print "accel"
        vitesse_theorique = vitesse_theorique + acceleration

    elif (round(vitesse_theorique - acceleration, 2) >= vitesse_consigne):
        print "deccel"
        vitesse_theorique = vitesse_theorique - acceleration



for i in range(0, 260):
    vitesse_courante = position_theorique - position_precedente;
    position_precedente = position_theorique

    asservissement_position()
    asservissement_vitesse()

    position_theorique += vitesse_theorique

# plt.plot(plot)
# plt.plot(plot2)
plt.plot(plot3)
plt.plot(plot4)



plt.show()

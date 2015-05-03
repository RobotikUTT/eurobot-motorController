from math import *
import matplotlib.pyplot as plt

Dtot = 3
Vmax = 0.5
A = 0.3
deltaT = 15*(10**(-3))


if ((Vmax**2)/A > Dtot):

    #Triangle
    Vmax = sqrt(Dtot*A)
    Ta = Vmax/A

    plot1 = [0] + [A*i*deltaT for i in range(0, int(Ta / deltaT))]
    plot2 = [Vmax-A*i*deltaT for i in range(0, int(Ta / deltaT))] + [ 0 ]

    plt.plot(plot1+plot2)


else:
    Ta = float(Vmax)/A
    Tb = (1/float(Vmax)) * (Dtot - Vmax*Ta)
    Ttot = 2*Ta + Tb

    plot1 = [0] + [A*i*deltaT for i in range(0, int(Ta / deltaT))]
    plot2 = [ Vmax for i in range(0, int((Tb-Ta) / deltaT)) ]
    plot3 = [ Vmax-1*A*i*deltaT for i in range(0, int(Ta / deltaT))] + [ 0 ]

    plt.plot(plot1 + plot2 + plot3)

plt.show()

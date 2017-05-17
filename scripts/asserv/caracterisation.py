# !/usr/bin/env python
# -*- coding: utf-8 -*-

from matplotlib import pyplot as plt
import numpy as np
from scipy import signal
import sympy as sy
from symfit import *

sy.init_printing()  # LaTeX like pretty printing for IPython

def lti_to_sympy(lsys, symplify=True):
    """ Convert Scipy's LTI instance to Sympy expression """
    s = sy.Symbol('s')
    G = sy.Poly(lsys.num, s) / sy.Poly(lsys.den, s)
    return sy.simplify(G) if symplify else G


def sympy_to_lti(xpr, s=sy.Symbol('s')):
    """ Convert Sympy transfer function polynomial to Scipy LTI """
    num, den = sy.simplify(xpr).as_numer_denom()  # expressions
    p_num_den = sy.poly(num, s), sy.poly(den, s)  # polynomials
    c_num_den = [sy.expand(p).all_coeffs() for p in p_num_den]  # coefficients
    l_num, l_den = [sy.lambdify((), c)() for c in c_num_den]  # convert to floats
    return signal.lti(l_num, l_den)


# Initialisation matplotlib
fig, ax = plt.subplots()

# On récupère les données sous la forme `[temps en ms] [vitesse en ticks.s-1]`
data = np.genfromtxt('motor.dat', delimiter=' ')
t_data = np.array([ data[i][0] for i in range(len(data)) ])
y_data = np.array([ data[i][1] for i in range(len(data)) ])

print y_data

# On trace la réponse
ax.plot(t_data, y_data, label='Original')   

# On cherche à caractériser les paramètres Tau (constante de temps) et K (gain)
# tel que le système soit un LTI 1er ordre 
x, t = variables('x, t')

# Valeur de départ pour l'algorithme d'optimisation. Elles doivent être du même ordre
# de grandeur que les valeurs réelles
Tau = Parameter(80)
K = Parameter(230)

model_dict = {
    D(x, t): -1.0/Tau * x + K/Tau
}

ode_model = ODEModel(model_dict, initial={ x: 0.0, t: 0.0 })

fit = Fit(ode_model, t=t_data, x=y_data)
fit_result = fit.execute()

Tau_m = fit_result.params.Tau
K_m = fit_result.params.K

print "Tau calculé: ", Tau_m, " K calculé: ", K_m

# On trace la réponse unitaire du système modélisé
sys_m = signal.lti(1, [Tau_m/K_m, 1.0/K_m])
step_response = sys_m.step(T=t_data)[1]
y_data = step_response
plt.plot(t_data, y_data, label='Carac')

# Affichage des courbes
legend = ax.legend()
plt.show()

# !/usr/bin/env python
# -*- coding: utf-8 -*-

from matplotlib import pyplot as plt
import numpy as np

# Initialisation matplotlib
fig, ax = plt.subplots()

# On récupère les données sous la forme `[temps en ms] [vitesse en ticks.s-1]`
data = np.genfromtxt('motor.dat', delimiter=' ')
t_data = np.array([ data[i][0] for i in range(len(data)) ])
y_data = np.array([ data[i][1] for i in range(len(data)) ])
pwm_data = np.array([ data[i][2] for i in range(len(data)) ])
err_data = np.array([ data[i][3] for i in range(len(data)) ])
obj_data = np.array([ data[i][4] for i in range(len(data)) ])

# On trace la réponse
ax.plot(t_data, y_data, label='y')   
# ax.plot(t_data, pwm_data, label='pwm')   
ax.plot(t_data, err_data, label='err')   
ax.plot(t_data, obj_data, label='objectif')   

legend = ax.legend()
plt.show()

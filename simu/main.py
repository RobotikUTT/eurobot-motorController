from DigitalPID import DigitalPID
from PremierOrdre import PremierOrdre, FF
from generateurConsigne import genPosCmd
from matplotlib import pyplot as plt
import math
from cmath import exp

# Initialisation matplotlib
fig, ax = plt.subplots()

# Constantes
DT         = 0.05                                # s
K          = 238.011474213
Tau        = 88.9635887765 * DT                  # DT^-1
DIAMETRE   =  52                                 # mm
RESOLUTION =  1024                               # tick
PRECISION  = ((math.pi*DIAMETRE) / RESOLUTION)   # mm.tick^-21
V_MAX      = int((750.0 / PRECISION))          # tick.DT^-1
A_MAX      = int((500.0 / PRECISION))          # tick.DT^-2
ORDER      = 2

moteur      = PremierOrdre(DT, K, Tau)
feedForward = FF(DT, K, Tau)

# kp = 0.8
# ki = 0
# kd = 2.2
# kp = 0.1
# ki = 0
# kd = 0
pid = DigitalPID(DT, kp, ki, kd)

t_data   = []
v_data   = []
pos_data = []
pos_cmd_data = []
v_cmd_data = []
err_data = []
pwm_data = []

posCmd, tF = genPosCmd(2, [int(1000.0/PRECISION), V_MAX, A_MAX])
lastCmd = 0

for i in range(int((tF+1)/DT)):
    t = i*DT
    cmd = int(1000.0/PRECISION)
    cmd = posCmd(t)

    v_cmd = cmd - lastCmd
    lastCmd = cmd

    err = cmd
    if (len(pos_data) > 0):
        err = err - pos_data[i-1]


    # FF
    if (i == 0):
        pwm = 0
    elif (i == 1):
        pwm = v_cmd/(K*(1-exp(-t/Tau).real))
    else:
        # pwm = (v_cmd-pos_data[-1]*exp(-t/Tau).real)/(K*(1-exp(-t/Tau).real))
        pwm = v_cmd/K
    pwm =  int(pwm * 255)

    # pid
    pwm = int(pid.process(err))

    # if (pwm > 255):
    #     pwm = 255
    # elif (pwm < -255):
    #     pwm = -255
    # elif (pwm > 30):
    #     pwm = 0
    # elif (pwm < -30):
    #     pwm = 0

    v = int(moteur.process(pwm/255.0))
    
    pos = 0
    if (len(pos_data) > 0):
        pos = pos_data[i-1]
    pos = pos + v

    t_data.append(t)
    v_data.append(v)
    pos_cmd_data.append(cmd)
    v_cmd_data.append(v_cmd)
    err_data.append(err)
    pwm_data.append(pwm)
    pos_data.append(pos)
    

pos_cmd_data = [ cmd * PRECISION for cmd in pos_cmd_data ]
pos_data = [ pos * PRECISION for pos in pos_data ]
plt.plot(t_data, pos_cmd_data, label='pos cmd')
plt.plot(t_data, v_cmd_data, label='v cmd')
plt.plot(t_data, v_data, label='v')
plt.plot(t_data, pos_data, label='pos')
# plt.plot(t_data, err_data)
# plt.plot(t_data, pwm_data, label='pwm')

# Affichage des courbes
legend = ax.legend()
plt.show()

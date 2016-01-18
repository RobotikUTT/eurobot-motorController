#!/usr/bin/python
import serial
import pylab as plt
import numpy as np
from random import random
from time import sleep
import threading

def update():
    ##########################
    #         Graph          #
    ##########################
    DT                = 15
    maxPoints         = 20 * DT
    X                 = []
    velocityMeasured  = []
    PWM               = []

    ax     = plt.axes()
    graph  = plt.plot(X, velocityMeasured, 'b')[0]
    graph2 = plt.plot(X, PWM, 'r')[0]

    plt.draw()

    ##########################
    #    Serial connection   #
    ##########################

    conn = serial.Serial('/dev/ttyUSB1', 115200)
    # Reset arduino
    conn.setDTR(False) # Drop DTR
    sleep(0.022)       # Read somewhere that 22ms is what the UI does.
    conn.setDTR(True)  # UP the DTR back

    ##########################
    #       Main loop        #
    ##########################

    leftGoal = 0
    goalLine = None
    goalDeltaMinLine = None
    goalDeltaMaxLine = None

    while True:
        # Read serial data
        data = conn.readline().rstrip()
        print(data)

        # Handle data
        if data == "Enter control params:":
            conn.write(raw_input("Left kp: ") + " " + raw_input("Left ki: ") + " " +
                raw_input("Left kd: ") + " " + raw_input("Right kp: ") + " " +
                raw_input("Right ki: ") + " " + raw_input("Right kd: "))
            conn.flush()

        elif data == "Enter control type:":
            conn.write("1") # velocity
            conn.flush()

        elif data == "Enter goals:":
            leftGoal  = float(raw_input("Left goal: "))
            rightGoal = float(raw_input("Right goal: "))
            conn.write(str(leftGoal) + " " + str(rightGoal))
            conn.flush()
            ax = plt.gca().set_ylim(0, 2 * max(leftGoal, rightGoal))
            goalLine = plt.hlines(max(leftGoal, rightGoal), 0, maxPoints, colors='g')
            goalDeltaMinLine = plt.hlines((max(leftGoal, rightGoal)-10), 0, maxPoints, colors='y', linestyle='-')
            goalDeltaMaxLine = plt.hlines((max(leftGoal, rightGoal)+10), 0, maxPoints, colors='y', linestyle='-')

        elif data == "Test finished":
            goalLine.remove()
            goalDeltaMinLine.remove()
            goalDeltaMaxLine.remove()
            X                = []
            velocityMeasured = []
            PWM              = []
            continue

        try:
            data = float(data)
        except:
            continue

        data2 = 0
        try:
            data2 = float(conn.readline().rstrip())
        except:
            pass

        velocityMeasured.append(data)
        PWM.append(data2)

        X = np.arange(len(velocityMeasured))

        graph.set_ydata(velocityMeasured)
        graph.set_xdata(X)
        graph2.set_ydata(PWM)
        graph2.set_xdata(X)

        plt.draw()

        # Rescale
        # ax = plt.gca()
        # ax.relim()
        # ax.autoscale_view()

def handle_close(evt):
    print('Figure closed !')

##########################
#      Entry point       #
##########################

plt.plot([],[])
t = threading.Thread(target= update)
t.setDaemon(True)
t.start()
plt.show()
from math import *
import matplotlib.pyplot as plt

plot = []
plot2 = []
plot3 = []
plot4 = []
plot5 = []

distanceVelocityMax = 1
distanceAcceleration = 0.01

actualDistanceVelocity = 0
actualDistance = 0
previousDistance = 0

distanceObjective = 150
distanceFreinage = distanceObjective / 2
distanceFreinageReset = False
velocityObjective = 0

theoricalVelocity = 0
theoricalDistance = 0


for i in range(0, 280):
    actualDistanceVelocity = theoricalDistance - previousDistance;
    previousDistance = theoricalDistance

    remainingDistance = distanceObjective - theoricalDistance

    done = False
    if (fabs(remainingDistance)  <= distanceAcceleration):
        # print "FINI"
        done = True

    if (done):
        velocityObjective = 0
    else:
        if ((remainingDistance <= pow(theoricalVelocity, 2) / (2 * distanceAcceleration))):
            velocityObjective = 0

        elif (remainingDistance > 0):
                velocityObjective = distanceVelocityMax
        elif (remainingDistance < 0):
                velocityObjective = -distanceVelocityMax


    print remainingDistance
    plot2.append(remainingDistance)
    plot.append(theoricalDistance)
    # plot3.append(theoricalVelocity)
    # plot4.append(velocityObjective)
    # plot5.append(theoricalDistance-remainingDistance)


    if (round(theoricalVelocity + distanceAcceleration, 4) <= velocityObjective):
        theoricalVelocity = theoricalVelocity + distanceAcceleration

    elif (round(theoricalVelocity - distanceAcceleration, 4) >= velocityObjective):
        theoricalVelocity = theoricalVelocity - distanceAcceleration


    theoricalDistance += theoricalVelocity

plt.plot(plot)
plt.plot(plot2)
# plt.plot(plot3)
# plt.plot(plot4)



plt.show()

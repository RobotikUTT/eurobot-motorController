from math import *
import matplotlib.pyplot as plt
f = open('asserv.txt', 'r')
points = f.read().split(',')
print str(len(points)) + "\n"
plt.plot(points)
plt.show()
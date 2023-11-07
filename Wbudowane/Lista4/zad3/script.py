import numpy as np
import math

a = np.arange(0, 6.283, 0.01)



def h(x):
    if x < 25 * math.pi:
        return 50 * math.sin(x / 50 + math.pi/2) + 50
    elif x > 50 * math.pi:
        return 50 * math.sin(x / 50 - math.pi) + 50
    else:
        return -50 * math.sin(x / 25 + math.pi) + 50


counter = 0

for i in np.arange(0, 75 * math.pi, 0.5):
    print(math.floor(h(i)), end=", ")
    counter += 1
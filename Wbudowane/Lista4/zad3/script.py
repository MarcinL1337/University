import numpy as np
import math

a = np.arange(0, 6.283, 0.01)



def h(x):
    if x < 500 * math.pi:
        return 1000 * math.sin(x / 1000 + math.pi/2) + 1000
    elif x > 1000 * math.pi:
        return 1000 * math.sin(x / 1000 - math.pi) + 1000
    else:
        return -1000 * math.sin(x / 500 + math.pi) + 1000


counter = 0

for i in np.arange(0, 1500 * math.pi, 3):
    print(math.floor(h(i)), end=", ")
    counter += 1

# print(counter)
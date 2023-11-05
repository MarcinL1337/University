import numpy as np
import math

a = np.arange(0, 6.283, 0.01)



def h(x):
    if x < 250 *  math.pi or x > 1250 * math.pi:
        return 1000 * math.sin(x / 500 + math.pi/2) + 1000
    else:
        return -1000 * math.sin(x / 1000 - math.pi/4) + 1000


# counter = 0

for i in range(1000):
    print(h(i), end=", ")

# print(counter)
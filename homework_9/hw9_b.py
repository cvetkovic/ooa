import math
import matplotlib.pyplot as plt
import numpy as np

w = []

f = open("/mnt/c/Users/jugos000/CLionProjects/ooa/cmake-build-debug/hw9_out.txt", "r")
for x in range(0, 10):
    w.append(float(f.readline()))

f.close()

x = np.arange(-1, 1.001, 0.001)
y_training = []
y_out = []

for x_i in x:
    y_t = 0.5 * math.sin(math.pi * x_i)
    y_o = 0

    for i in range(0, 5):
        y_o += w[i + 5] * np.tanh(w[i] * x_i)
    y_o = np.tanh(y_o)

    y_training.append(y_t)
    y_out.append(y_o)

plt.plot(x, y_training)
plt.plot(x, y_out)
plt.xlabel('x')
plt.legend(['y_training', 'y_out'])
plt.grid()
plt.show()

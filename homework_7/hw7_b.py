import matplotlib.pyplot as plt
import numpy as np

for x in range(1, 20):
    f1 = open("/mnt/c/Users/jugos000/CLionProjects/ooa/cmake-build-debug/average_" + str(x) + ".txt", "r")
    f2 = open("/mnt/c/Users/jugos000/CLionProjects/ooa/cmake-build-debug/minimum_" + str(x) + ".txt", "r")

    average = np.array(f1.readlines()).astype(np.float)
    minimum = np.array(f2.readlines()).astype(np.float)

    plt.subplot(211)
    plt.plot(average)
    plt.grid()

    plt.subplot(212)
    plt.plot(minimum)
    plt.grid()

    f1.close()
    f2.close()

plt.subplot(211)
plt.xlabel('Iteration')
plt.ylabel('Cumulative minimum of cost function')

plt.subplot(212)
plt.xlabel('Iteration')
plt.ylabel('Average cumulative minimum of cost function')

plt.show()

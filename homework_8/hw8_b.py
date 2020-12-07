import matplotlib.pyplot as plt
import numpy as np

minimum = []

plt.figure()

for x in range(1, 20):
    f = open("/mnt/c/Users/jugos000/CLionProjects/ooa/cmake-build-debug/minimum_" + str(x) + ".txt", "r")

    minimum.append(np.array(f.readlines()).astype(np.float))

    plt.loglog(minimum[x - 1])

    f.close()

plt.xlabel('Iteration')
plt.ylabel('Cumulative minimum of cost function')
plt.grid()
plt.show()

averagedMinimum = []
for i in range(0, minimum[0].size):
    rowSum = 0
    for j in range(0, 19):
        rowSum += minimum[j][i]
    rowSum /= 20

    averagedMinimum.append(rowSum)

plt.figure()
plt.plot(averagedMinimum)
plt.xlabel('Iteration')
plt.ylabel('Average cumulative minimum of cost function')
plt.grid()
plt.show()

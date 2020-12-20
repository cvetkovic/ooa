import matplotlib.pyplot as plt

x1 = []
x2 = []
f1 = []
f2 = []

f = open("/mnt/c/Users/jugos000/CLionProjects/ooa/cmake-build-debug/hw11.txt", "r")
for x in range(0, 10000):
    line = f.readline()
    s = line.split(' ');

    x1.append(float(s[0]))
    x2.append(float(s[1]))
    f1.append(float(s[2]))
    f2.append(float(s[3]))

f.close()

plt.axes(projection='3d')
plt.scatter(x1, x2, f1)
plt.grid()
plt.show()

plt.figure()
plt.grid()
plt.plot(f1, f2, 'rx', markersize=1)
plt.xlabel(r'$f_{1}$')
plt.ylabel(r'$f_{2}$')
plt.show()

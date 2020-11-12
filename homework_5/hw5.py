import math
import matplotlib.pyplot as plt
import numpy as np

from scipy.optimize import minimize

############################################

Rc = 1000
Re = 1000
Vcc = 12
Is = 1e-13
Vt = 0.026
beta_f = 100
beta_r = 2


############################################

def optimization_function(x):
    Vbe = x[1]
    Vbc = x[2]

    Ic = Is * ((math.exp(Vbe / Vt) - math.exp(Vbc / Vt)) - (math.exp(Vbc / Vt) - 1) / beta_r)
    Ib = Is * ((math.exp(Vbe / Vt) - 1) / beta_f + (math.exp(Vbc / Vt) - 1) / beta_r)
    Ie = Ic + Ib

    term1 = E - Vbe - Re * Ie
    term2 = Vcc - Rc * Ic + Vbc - E

    return abs(term1) + abs(term2)


############################################

def optimization(E):
    x = np.array([1, 1, 1])
    res = minimize(optimization_function, x, method='nelder-mead', options={'xatol': 1e-8, 'disp': True})
    print(res.x)

    Vbe = res.x[1]
    Vbc = res.x[2]
    Vce = Vbe - Vbc
    Ic = Is * ((math.exp(Vbe / Vt) - math.exp(Vbc / Vt)) - (math.exp(Vbc / Vt) - 1) / beta_r)
    Ib = Is * ((math.exp(Vbe / Vt) - 1) / beta_f + (math.exp(Vbc / Vt) - 1) / beta_r)
    Ie = Ic + Ib

    return np.array([Vbe, Vbc, Vce, Ic, Ib, Ie, res.fun])


############################################

E = 5
res = optimization(E)
print('Initial run (E = 5)')
print('---------------------------------------------------')
print('Vbe: ', "{:.6f}".format(res[0]), ' V')
print('Vbc: ', "{:.6f}".format(res[1]), ' V')
print('Vce: ', "{:.6f}".format(res[2]), ' V')
print('Ic:  ', "{:.6f}".format(res[3]), ' A')
print('Ib:  ', "{:.6f}".format(res[4]), ' A')
print('Ie:  ', "{:.6f}".format(res[5]), ' A')
print('f:   ', "{:.6f}".format(res[6]))
print('---------------------------------------------------')

print('---------------------------------------------------')
Vbe_array = np.array([])
Ic_array = np.array([])
Vout_array = np.array([])

for E in np.arange(0.0, 10.0, 0.1):
    res = optimization(E)
    print('Traversal run (E =', "{:.6f}".format(E), ')')
    print('---------------------------------------------------')
    print('Vbe: ', "{:.6f}".format(res[0]), ' V')
    print('Vbc: ', "{:.6f}".format(res[1]), ' V')
    print('Vce: ', "{:.6f}".format(res[2]), ' V')
    print('Ic:  ', "{:.6f}".format(res[3]), ' A')
    print('Ib:  ', "{:.6f}".format(res[4]), ' A')
    print('Ie:  ', "{:.6f}".format(res[5]), ' A')
    print('f:   ', "{:.6f}".format(res[6]))
    print('---------------------------------------------------')

    Vbe_array = np.append(Vbe_array, res[0])
    Ic_array = np.append(Ic_array, res[3])
    Vout_array = np.append(Vout_array, Re * res[5] + res[2])

plt.subplot(311)
plt.plot(np.arange(0.0, 10.0, 0.1), Vbe_array)
plt.grid()
plt.xlabel(r'$E [V]$')
plt.ylabel(r'$V_{be} [V]$')

plt.subplot(312)
plt.plot(np.arange(0.0, 10.0, 0.1), Ic_array)
plt.grid()
plt.xlabel(r'$E [V]$')
plt.ylabel(r'$I_{c} [V]$')

plt.subplot(313)
plt.plot(np.arange(0.0, 10.0, 0.1), Vout_array)
plt.grid()
plt.xlabel(r'$E [V]$')
plt.ylabel(r'$V_{out} [V]$')

plt.show()

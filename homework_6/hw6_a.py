from scipy.optimize import linprog

c = [-20, -30]
A = [[1, 3], [3, 1], [1, 6]]
b = [75, 99, 288]
x0_bounds = (0, None)
x1_bound = (0, None)

res = linprog(c, A, b, bounds=[x0_bounds, x1_bound])

print('a = ', res.x[0], ', b = ', res.x[1])
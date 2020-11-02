//
// Created by jugos000 on 02-Nov-20.
//

#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

constexpr double epsilon = 1e-12;

double bisectionSolver(const unsigned int order, double a, double b) {
    if (sph_bessel(order, a) * sph_bessel(order, b) >= 0)
        throw runtime_error("Bisection method cannot be applied because f(a)*f(b) >= 0.");

    double fa = sph_bessel(order, a);

    while (true) {
        double x = (a + b) * 0.5;
        double fx = sph_bessel(order, x);

        if (abs(fx) <= epsilon)
            return x;

        if (fa * fx < 0.0)
            b = x;
        else
            a = x;
    }
}

int main(int argc, char **argv) {
    const int precision = 15;
    double o1_x1 = bisectionSolver(1, 2, 6);
    double o1_x2 = bisectionSolver(1, 6, 8);

    cout << "Order 1 - x1 = " << setprecision(precision) << o1_x1 << endl;
    cout << "Order 1 - x2 = " << setprecision(precision) << o1_x2 << endl << endl;

    double o2_x1 = bisectionSolver(2, 3, 6);
    double o2_x2 = bisectionSolver(2, 6, 11);

    cout << "Order 2 - x1 = " << setprecision(precision) << o2_x1 << endl;
    cout << "Order 2 - x2 = " << setprecision(precision) << o2_x2 << endl;

    return 0;
}
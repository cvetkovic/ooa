//
// Created by jugos000 on 09-Nov-20.
//

#include <cmath>
#include <iostream>
#include <iomanip>

#include "NelderMead.cpp"

using namespace std;

constexpr double Rc = 1000;
constexpr double Re = 1000;
//constexpr double E = 5;
constexpr double Vcc = 12;
constexpr double Is = 1e-13;
constexpr double Vt = 0.026;
constexpr double beta_f = 100;
constexpr double beta_r = 2;

double optimizationFunction(double E, double x[]) {
    const double Vbe = x[1];
    const double Vbc = x[2];

    double Ic = Is * ((exp(Vbe / Vt) - exp(Vbc / Vt)) - (exp(Vbc / Vt) - 1) / beta_r);
    double Ib = Is * ((exp(Vbe / Vt) - 1) / beta_f + (exp(Vbc / Vt) - 1) / beta_r);
    double Ie = Ic + Ib;

    double term1 = E - Vbe - Re * Ie;
    double term2 = Vcc - Rc * Ic + Vbc - E;

    return abs(term1) + abs(term2);
}

#ifndef TEST
int main(int argc, char **argv) {
    for (double E = 0; E <= 5; E += 0.1) {
        cout << "------- E = " << E << " V -------------------------------------" << endl;
        // memory allocation
        double **p = new double *[N + 2];
        for (int i = 0; i < N + 2; i++)
            p[i] = new double[N + 1];
        double *y = new double[N + 2];
        double ftol = (double) 1e-5;
        int nfunk;

        // initial simplex
        for (int i = 0; i < N + 2; i++) {
            for (int j = 0; j < N + 1; j++) {
                p[i][j] = 0.0;
                if (i > 1 && j == i - 1)
                    p[i][i - 1] = 1.0;
            }
            y[i] = f(p[i]);
        }

        // optimization
        NMsimplex(E, p, y, N, ftol, optimizationFunction, &nfunk);

        // output
        for (int i = 1; i < N + 1; i++)
            printf("%2.5f ", p[1][i]);
        printf("f(xmin)=%2.2f", y[1]);
        printf("\n");
        printf("%d\n", nfunk);

        double Vbe = p[1][1];
        double Vbc = p[1][2];
        double Vce = Vbe - Vbc;
        double Ic = Is * ((exp(Vbe / Vt) - exp(Vbc / Vt)) - (exp(Vbc / Vt) - 1) / beta_r);
        double Ib = Is * ((exp(Vbe / Vt) - 1) / beta_f + (exp(Vbc / Vt) - 1) / beta_r);
        double Ie = Ic + Ib;

        cout << "Vbe: " << setprecision(5) << Vbe << " V" << endl;
        cout << "Vbc: " << setprecision(5) << Vbc << " V" << endl;
        cout << "Vce: " << setprecision(5) << Vce << " V" << endl;
        cout << "Ic: " << setprecision(5) << Ic << " A" << endl;
        cout << "Ib: " << setprecision(5) << Ib << " A" << endl;
        cout << "Ie: " << setprecision(5) << Ie << " A" << endl;

        // clean-up
        for (int i = 0; i < N; i++)
            delete[] p[i];
        delete[] p;

        cout << "---------------------------------------------" << endl;
    }

    return 0;
}
#endif
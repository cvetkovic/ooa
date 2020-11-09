//
// Created by jugos000 on 09-Nov-20.
//

#include <cmath>
#include <iostream>
#include <iomanip>

#include "NelderMead.cpp"

using namespace std;

constexpr float Rc = 1000;
constexpr float Re = 1000;
constexpr float E = 5;
constexpr float Vcc = 12;
constexpr float Is = 1e-13;
constexpr float Vt = 0.026;
constexpr float beta_f = 100;
constexpr float beta_r = 2;

float optimizationFunction(float x[]) {
    const float Vbe = x[1];
    const float Vbc = x[2];

    float Ic = Is * ((exp(Vbe / Vt) - exp(Vbc / Vt)) - (exp(Vbc / Vt) - 1) / beta_r);
    float Ib = Is * ((exp(Vbe / Vt) - 1) / beta_f + (exp(Vbc / Vt) - 1) / beta_r);
    float Ie = Ic + Ib;

    float term1 = E - Vbe - Re * Ie;
    float term2 = Vcc - Rc * Ic + Vbc - E;

    return abs(term1) + abs(term2);
}

int main(int argc, char **argv) {

    // memory allocation
    float **p = new float *[N + 2];
    for (int i = 0; i < N + 2; i++)
        p[i] = new float[N + 1];
    float *y = new float[N + 2];
    float ftol = (float) 1e-5;
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

    float E = 0;

    // optimization
    NMsimplex(p, y, N, ftol, optimizationFunction, &nfunk);

    // output
    for (int i = 1; i < N + 1; i++)
        printf("%2.5f ", p[1][i]);
    printf("f(xmin)=%2.2f", y[1]);
    printf("\n");
    printf("%d\n", nfunk);

    float Vbe = p[1][1];
    float Vbc = p[1][2];
    float Vce = Vbe - Vbc;
    float Ic = Is * ((exp(Vbe / Vt) - exp(Vbc / Vt)) - (exp(Vbc / Vt) - 1) / beta_r);
    float Ib = Is * ((exp(Vbe / Vt) - 1) / beta_f + (exp(Vbc / Vt) - 1) / beta_r);
    float Ie = Ic + Ib;

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

    return 0;
}
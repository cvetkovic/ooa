//
// Created by jugos000 on 20-Dec-20.
//

#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <random>

using namespace std;

constexpr long NUMBER_OF_POINTS = 10000;

inline double f1(const double x1, const double x2) {
    return pow(x1, 2) + pow(x2, 2);
}

inline double f2(const double x1, const double x2) {
    return -pow((x1 - x2), 2);
}

inline bool constraint1(const double x) {
    constexpr double lowerLimit = -1.0;
    constexpr double upperLimit = 1.0;

    if (lowerLimit <= x && x <= upperLimit)
        return true;
    else
        return false;
}

inline bool constraint2(const double x1, const double x2) {
    constexpr double c = 1.0 / 4.0;

    if (x1 * x2 + c >= 0)
        return true;
    else
        return false;
}

random_device rd;
mt19937 mt(rd());

int main() {
    uniform_real_distribution<double> randomGenerator(-2, 2);

    ofstream file;
    file.open("hw11.txt");

    for (long p = 0; p < NUMBER_OF_POINTS;) {
        const double x1 = randomGenerator(mt);
        const double x2 = randomGenerator(mt);

        bool satisfiesConstraint = true;
        if (!constraint1(x1) || !constraint1(x2))
            satisfiesConstraint = false;
#ifdef ADDITIONAL_CONSTRAINT
        if (!constraint2(x1, x2))
            satisfiesConstraint = false;
#endif

        double f1_value = f1(x1, x2);
        double f2_value = f2(x1, x2);

        if (satisfiesConstraint) {
            file << fixed << setprecision(5) << x1 << " " <<
                 fixed << setprecision(5) << x2 << " " <<
                 fixed << setprecision(5) << f1_value << " " <<
                 fixed << setprecision(5) << f2_value << endl;

            p++;
        }
    }

    file.close();

    return 0;
}
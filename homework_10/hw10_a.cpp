//
// Created by jugos000 on 18-Dec-20.
//

#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

constexpr double w = 0.729;
constexpr double c1 = 1.494;
constexpr double c2 = 1.494;
constexpr double v_max = 0.2;
constexpr double dt = 1;

constexpr long NUMBER_OF_ITERATIONS = 100000;
constexpr long SWARM_DIMENSION = 2000;

random_device rd;
mt19937 mt(rd());

struct point {
    double x;
    double y;
    double z;

    point(double x, double y, double z) : x(x), y(y), z(z) {}

    point operator+(const point &p) {
        return point(p.x + x, p.y + y, p.z + z);
    }

    point operator*(double w) {
        return point(w * x, w * y, w * z);
    }

    friend ostream &operator<<(ostream &os, const point &p) {
        os << "(" << fixed << setprecision(2) << p.x << ", " << fixed << setprecision(2) << p.y << ", " << fixed << setprecision(2) << p.z << ")";
        return os;
    }
};

struct solution {
    const point A;
    const point B;
    const point C;
    const point D;
    point S1;
    point S2;

    point bestS1;
    point bestS2;
    double bestScore = numeric_limits<double>::max();

    point v1;
    point v2;

    solution(const point &s1, const point &s2, const point &v1, const point &v2) :
            S1(s1),
            S2(s2),
            v1(v1),
            v2(v2),
            bestS1(s1),
            bestS2(s2),
            A(1, 5, 1),
            B(3, 2, 0),
            C(5, 6, 1),
            D(6, 3, 3) {}

    void updateSpeed(const solution &g) {
        static uniform_real_distribution<double> r(0, 1);

        v1.x = w * v1.x + c1 * r(mt) * (bestS1.x - S1.x) + c2 * r(mt) * (g.S1.x - S1.x);
        v1.y = w * v1.y + c1 * r(mt) * (bestS1.y - S1.y) + c2 * r(mt) * (g.S1.y - S1.y);
        v1.z = w * v1.z + c1 * r(mt) * (bestS1.z - S1.z) + c2 * r(mt) * (g.S1.z - S1.z);

        v2.x = w * v2.x + c1 * r(mt) * (bestS2.x - S2.x) + c2 * r(mt) * (g.S2.x - S2.x);
        v2.y = w * v2.y + c1 * r(mt) * (bestS2.y - S2.y) + c2 * r(mt) * (g.S2.y - S2.y);
        v2.z = w * v2.z + c1 * r(mt) * (bestS2.z - S2.z) + c2 * r(mt) * (g.S2.z - S2.z);
    }

    void updatePosition() {
        S1.x = S1.x + dt * v1.x;
        S1.y = S1.y + dt * v1.y;
        S1.z = S1.z + dt * v1.z;

        S2.x = S2.x + dt * v2.x;
        S2.y = S2.y + dt * v2.y;
        S2.z = S2.z + dt * v2.z;
    }
};

inline double euclideanDistance(const point &p1, const point &p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

inline double optimizationFunction(const solution &s) {
    double a_s1 = euclideanDistance(s.A, s.S1);
    double b_s1 = euclideanDistance(s.B, s.S1);
    double s1_s2 = euclideanDistance(s.S1, s.S2);
    double c_s2 = euclideanDistance(s.C, s.S2);
    double d_s2 = euclideanDistance(s.D, s.S2);

    return a_s1 + b_s1 + s1_s2 + c_s2 + d_s2;
}

int main() {
    // AGENT -> EQ. individual
    // SWARM -> EQ. population
    solution g(point(1, 1, 1),
               point(1, 1, 1),
               point(0.01, 0.01, 0.01),
               point(0.01, 0.01, 0.01));
    long iteration = 0;

    solution **swarm = new solution *[SWARM_DIMENSION];

    for (int i = 0; i < SWARM_DIMENSION; i++) {
        static uniform_real_distribution<double> xyGenerator(0, 10);
        static uniform_real_distribution<double> zGenerator(0, 4);

        swarm[i] = new solution(point(xyGenerator(mt), xyGenerator(mt), zGenerator(mt)),
                                point(xyGenerator(mt), xyGenerator(mt), zGenerator(mt)),
                                point(0.01, 0.01, 0.01),
                                point(0.01, 0.01, 0.01));
    }

    while (iteration < NUMBER_OF_ITERATIONS) {
        for (int i = 0; i < SWARM_DIMENSION; i++) {
            solution &s = *swarm[i];

            double cost = optimizationFunction(s);
            if (cost < s.bestScore) {
                s.bestScore = cost;
                s.bestS1 = s.S1;
                s.bestS2 = s.S2;
            }

            if (cost < g.bestScore) {
                g.bestScore = cost;
                g.bestS1 = s.S1;
                g.bestS2 = s.S2;
            }

            s.updateSpeed(g);
            s.updatePosition();
        }

        iteration++;

        if (iteration % 10000 == 0) {
            cout << "Progress: " << fixed << setprecision(3) << 100.0 * iteration / NUMBER_OF_ITERATIONS << "%" << endl;
            cout << "Current minimum: " << fixed << setprecision(3) << g.bestScore << endl;
        }
    }

    cout << "S1" << g.bestS1 << ", " << "S2" << g.bestS2 << endl;
    cout << "Minimum: " << fixed << setprecision(3) << g.bestScore;

    for (int i = 0; i < SWARM_DIMENSION; i++)
        delete swarm[i];
    delete[] swarm;

    return 0;
}
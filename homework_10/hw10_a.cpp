//
// Created by jugos000 on 20-Dec-20.
//

#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <unordered_map>

using namespace std;

constexpr double w = 0.729;
constexpr double c1 = 1.494;
constexpr double c2 = 1.494;
constexpr double v_max = 0.2;
constexpr double dt = 1;

constexpr long NUMBER_OF_ITERATIONS = 50;
constexpr long SWARM_DIMENSION = 2000;

random_device rd;
mt19937 mt(rd());

struct Point {
    double x;
    double y;
    double z;

    Point() {}

    constexpr Point(double x, double y, double z) : x(x), y(y), z(z) {}

    friend ostream &operator<<(ostream &os, const Point &p) {
        os << "(" << fixed << setprecision(3) << p.x << ", " << fixed << setprecision(3) << p.y << ", " << fixed
           << setprecision(3) << p.z << ")";
        return os;
    }

    friend Point operator+(const Point &p1, const Point &p2) {
        return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
    }

    friend Point operator-(const Point &p1, const Point &p2) {
        return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
    }

    friend Point operator*(const double c, const Point &p) {
        return Point(c * p.x, c * p.y, c * p.z);
    }

    static double euclideanDistance(const Point &p1, const Point &p2) {
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
    }

    static Point generateRandomPoint() {
        static uniform_real_distribution<double> randomGeneratorXY(0, 10);
        static uniform_real_distribution<double> randomGeneratorZ(0, 4);

        return Point(randomGeneratorXY(mt), randomGeneratorXY(mt), randomGeneratorZ(mt));
    }

    static Point generateRandomSpeed() {
        //static uniform_real_distribution<double> randomGenerator(0, 0.01);

        return Point(0.01, 0.01, 0.01);
    }
};

struct Vector;

inline double costFunction(const Vector &v);

struct Vector {
    Point S1;
    Point S2;

    Vector() {}

    Vector(const Point &s1, const Point &s2) : S1(s1), S2(s2) {}

    friend Vector operator+(const Vector &v1, const Vector &v2) {
        return Vector(v1.S1 + v2.S1, v1.S2 + v2.S2);
    }

    friend Vector operator-(const Vector &v1, const Vector &v2) {
        return Vector(v1.S1 - v2.S1, v1.S2 - v2.S2);
    }

    friend Vector operator*(const double c, const Vector &v) {
        return Vector(c * v.S1, c * v.S2);
    }

    friend bool operator<(const Vector &v1, const Vector &v2) {
        double f1 = costFunction(v1);
        double f2 = costFunction(v2);

        return f1 < f2;
    }

    friend ostream &operator<<(ostream &os, const Vector &v) {
        os << "(S1, S2, cost) = (" << v.S1 << ", " << v.S2 << ", " << fixed << setprecision(3) << costFunction(v)
           << ")";
        return os;
    }
};

constexpr Point A(1.0, 5.0, 1.0);
constexpr Point B(3.0, 2.0, 0.0);
constexpr Point C(5.0, 6.0, 1.0);
constexpr Point D(6.0, 3.0, 3.0);

inline double costFunction(const Vector &v) {
    double a_s1 = Point::euclideanDistance(A, v.S1);
    double b_s1 = Point::euclideanDistance(B, v.S1);
    double s1_s2 = Point::euclideanDistance(v.S1, v.S2);
    double c_s2 = Point::euclideanDistance(C, v.S2);
    double d_s2 = Point::euclideanDistance(D, v.S2);

    return a_s1 + b_s1 + s1_s2 + c_s2 + d_s2;
}

int main() {
    Vector bestSolution;

    array<Vector, SWARM_DIMENSION> particles;
    array<Vector, SWARM_DIMENSION> particleSpeed;
    unordered_map<Vector *, Vector> particleBestVector;


    for (int i = 0; i < SWARM_DIMENSION; i++) {
        Vector v(Point::generateRandomPoint(), Point::generateRandomPoint());
        Vector s(Point::generateRandomSpeed(), Point::generateRandomSpeed());

        particles[i] = v;
        particleBestVector[&particles[i]] = v;
        particleSpeed[i] = s;

        if (v < bestSolution)
            bestSolution = v;
    }

    for (int it = 0; it < NUMBER_OF_ITERATIONS; it++) {
        for (int i = 0; i < SWARM_DIMENSION; i++) {
            double cost = costFunction(particles[i]);
            if (cost < costFunction(particleBestVector[&particles[i]]))
                particleBestVector[&particles[i]] = particles[i];
            if (particles[i] < bestSolution)
                bestSolution = particles[i];

            static uniform_real_distribution<double> random01(0.0, 1.0);

            Vector &velocity = particleSpeed[i];
            Vector v1 = w * velocity;
            Vector v2 = (c1 * random01(mt)) * (particleBestVector[&particles[i]] - particles[i]);
            Vector v3 = (c2 * random01(mt)) * (bestSolution - particles[i]);
            particleSpeed[i] = v1 + v2 + v3;

            particles[i] = particles[i] + particleSpeed[i];
        }
    }

    cout << bestSolution << endl;

    return 0;
}
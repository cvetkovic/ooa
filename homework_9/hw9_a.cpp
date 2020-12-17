//
// Created by jugos000 on 17-Dec-20.
//

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

constexpr double PI = 3.14159265358979323846;

constexpr int D = 10;

constexpr int POPULATION_SIZE = 50;
static_assert(POPULATION_SIZE >= 4,
              "Population size must be greater or equal to 3 for differential evolution algorithm.");
constexpr int NUMBER_OF_GENERATIONS = 50;

random_device rd;
mt19937 mt(rd());

struct w_vector {
    double w[D];
};

struct population {
    w_vector vectors[POPULATION_SIZE];
    w_vector z_vectors[POPULATION_SIZE];
};

inline double y_training(double x) {
    if (x < -1 || x > 1)
        throw runtime_error("Invalid argument");

    return 0.5 * sin(PI * x);
}

inline double a(double x) {
    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}

inline double y_out(double x, const w_vector &wv) {
    double sum = 0;
    for (int i = 0; i < 5; i++)
        sum += wv.w[i + 5] * a(wv.w[i] * x);

    return a(sum);
}

inline double optimizationFunction(const w_vector &wv) {
    double sum = 0;

    for (double x_in = -1; x_in <= 1; x_in += 0.1)
        sum += pow(y_out(x_in, wv) - y_training(x_in), 2);

    return sqrt(sum);
}

inline void selection(population &p) {
    static uniform_real_distribution<double> F_generator(0, 2);
    static uniform_int_distribution<int> index_generator(0, NUMBER_OF_GENERATIONS - 1);

    for (int i = 0; i < POPULATION_SIZE; i++) {
        w_vector z;
        double F = F_generator(mt);

        // SELECTION
        // pick three vectors xa, xb, xc (xa != xb != xc)
        // F [0, 2]
        // z = xa + F * (xb - xc)

        while (true) {
            bool again = false;

            int a_index, b_index, c_index;
            a_index = index_generator(mt);
            do {
                b_index = index_generator(mt);
            } while (a_index == b_index);
            do {
                c_index = index_generator(mt);
            } while (a_index == c_index || b_index == c_index);

            w_vector *xa = &p.vectors[a_index];
            w_vector *xb = &p.vectors[b_index];
            w_vector *xc = &p.vectors[c_index];

            for (int i = 0; i < D; i++) {
                z.w[i] = (*xa).w[i] + F * ((*xb).w[i] - (*xc).w[i]);
                if (z.w[i] < -10 || z.w[i] > 10) {
                    again = true;
                    break;
                }
            }

            if (again == false)
                break;
        }

        p.z_vectors[i] = z;
    }
}

inline void crossover(population &p) {
    static uniform_int_distribution<int> R_generator(0, D - 1);
    static uniform_real_distribution<double> r_generator(0, 1);

    for (int i = 0; i < POPULATION_SIZE; i++) {
        double CR = r_generator(mt);

        // CROSSOVER
        // CR [0, 1]
        // rnd -> R [1, D] D = num_dim

        w_vector &x = p.vectors[i];
        w_vector &z = p.z_vectors[i];
        w_vector y;

        int R = R_generator(mt);

        for (int j = 0; j < D; j++) {
            double r_i = r_generator(mt);

            if (r_i < CR || j == R)
                y.w[j] = z.w[j];
            else
                y.w[j] = x.w[j];
        }

        double f_y = optimizationFunction(y);
        double f_x = optimizationFunction(x);
        if (f_y < f_x)
            p.vectors[i] = y;
    }
}

int main() {
    uniform_real_distribution<double> w_generator(-10, 10);

    // randomization of initial solutions
    population p;
    for (auto &vector : p.vectors)
        for (int j = 0; j < D; j++)
            vector.w[j] = w_generator(mt);

    double minimumScore = numeric_limits<double>::max();
    w_vector *minimumSolution;
    do {
        for (int gen = 0; gen < NUMBER_OF_GENERATIONS; gen++) {
            selection(p);
            crossover(p);
        }

        for (int i = 0; i < POPULATION_SIZE; i++) {
            double f = optimizationFunction(p.vectors[i]);
            if (f < minimumScore) {
                minimumSolution = &p.vectors[i];
                minimumScore = f;
            }
        }
    } while (minimumScore > 0.01);

    cout << "[";
    for (int i = 0; i < D; i++) {
        cout << (*minimumSolution).w[i];

        if (i != D - 1)
            cout << ", ";
        else
            cout << "]" << endl;
    }
    cout << "Minimum score: " << minimumScore << endl;

    ofstream f;
    f.open("hw9_out.txt");
    for (int i = 0; i < D; i++)
        f << fixed << setprecision(15) << (*minimumSolution).w[i] << endl;
    f.close();

    return 0;
}
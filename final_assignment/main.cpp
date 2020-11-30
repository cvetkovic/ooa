//
// Created by jugos000 on 26-Nov-20.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "shapes.hpp"

using namespace std;

constexpr int N = 23;
constexpr uint64_t maximumIterations = 100;

// 1 + 2 + ... 23 = 276

inline int optimizationFunction(const Solution &solution) {
    int result = canvasArea;

    for (auto &rectangle : shapes)
        result -= rectangle.calculateArea();

    return result;
}

void writeToFile(const Solution &solution) {
    ofstream file("fa_layout.txt");

    file << solution.getCanvasSizeX() << endl;
    file << solution.getCanvasSizeY() << endl;

    for (int i = 0; i < N; i++)
        file << solution.getRectangles()[i] << endl;

    file.close();
}

int main(int argc, char **argv) {

#ifdef CLASS_TEST
    Rectangle r(Point(10,10), 5, 6);
    cout << "P = " << r.calculateArea() << endl;
    cout << "Top right: " << r.getTopRight() << endl;
    r.rotate();
    cout << "Top right: " << r.getTopRight() << endl;
#endif

    constexpr double alpha = 0.95;
    int64_t T = 32 * 1024 * 1024;
    uint64_t iteration = 0;

    Solution current(N);
    int64_t currentCost = optimizationFunction(current);
    int64_t globalMin = currentCost;

    while (iteration++ < maximumIterations) {

        int64_t newCost = optimizationFunction(current);
        int64_t dE = newCost - currentCost;
        if (dE < 0) {
            currentCost = newCost;
        } else {
            double p = (dE == 0 ? 0.5 : exp(-1.0 * dE / T));
            // TODO: port to C++11
            double rnd = (double) (rand() / RAND_MAX);

            if (rnd < p) {
                currentCost = newCost;
            }
        }

        T *= alpha;

        if (currentCost < globalMin) {
            globalMin = currentCost;
        }
    }

    //writeToFile(initial);

    return 0;
}
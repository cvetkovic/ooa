//
// Created by jugos000 on 26-Nov-20.
//

#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>

#include "shapes.hpp"

using namespace std;

constexpr int N = 23;
constexpr int maximumIterations = 1000;

// 1 + 2 + ... 23 = 276

inline double euclideanDistance(const Point &p1, const Point &p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

inline double minimumDistanceBetweenVertices(const Rectangle &r1, const Rectangle &r2) {
    vector<Point> pointsA, pointsB;

    pointsA.push_back(r1.getTopLeft());
    pointsA.push_back(r1.getTopRight());
    pointsA.push_back(r1.getBottomLeft());
    pointsA.push_back(r1.getBottomRight());

    pointsB.push_back(r2.getTopLeft());
    pointsB.push_back(r2.getTopRight());
    pointsB.push_back(r2.getBottomLeft());
    pointsB.push_back(r2.getBottomRight());

    double min = numeric_limits<double>::max();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double distance = euclideanDistance(pointsA[i], pointsB[j]);

            if (distance < min)
                min = distance;
        }
    }

    return min;
}

inline double optimizationFunction(const Solution &solution) {
    double score = 0;
    bool visited[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j || visited[i][j] || visited[j][i])
                continue;

            const Rectangle &r1 = solution.getRectangles()[i];
            const Rectangle &r2 = solution.getRectangles()[j];

            score += minimumDistanceBetweenVertices(r1, r2);

            visited[i][j] = visited[j][i] = true;
        }
    }

    return score;
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
    ////////////////////////////
    // test 1
    ////////////////////////////
    Rectangle r(Point(10,10), 5, 6);
    cout << "P = " << r.calculateArea() << endl;
    cout << "Top right: " << r.getTopRight() << endl;
    r.rotate();
    cout << "Top right: " << r.getTopRight() << endl;

    ////////////////////////////
    // test 2
    ////////////////////////////
    Rectangle r1(Point(39, 18), 14, 10);
    Rectangle r2(Point(51, 12), 22, 2);
    cout << Rectangle::overlap(r1, r2) << endl; // should be 1

    Rectangle r3(Point(5, 5), 1, 1);
    Rectangle r4(Point(10, 10), 1, 1);
    cout << Rectangle::overlap(r3, r4);          // should be 0
#endif

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(0, 1);

    const int numberOfSolutions = 25;
    Solution **solutions = new Solution *[numberOfSolutions];

//#pragma omp parallel for
    for (int i = 0; i < numberOfSolutions; i++) {
        try {
            double T_0 = 32 * 1024 * 1024;
            int iteration = 0;

            Solution current(N);
            current.generateRandomLayout();

            double currentCost = optimizationFunction(current);
            double globalMin = currentCost;

            Solution globalSolution(current);

            constexpr int hammingMax = 24;
            constexpr int hammingMin = 1;
#ifdef LOGARITMIC_COOLING_SCHEDULE
            double T = T_0 / log(2);
#else
            double T = T_0;
#endif
            while (iteration < maximumIterations && T > 0.1) {
                int distance = (hammingMin - hammingMax) * iteration / (maximumIterations - 1) + hammingMax;
                Solution tmp(current);
                tmp.hamming(distance);

                int64_t newCost = optimizationFunction(tmp);
                int64_t dE = newCost - currentCost;
                if (dE < 0) {
                    globalSolution = current = tmp;
                    globalMin = currentCost = newCost;
                    current.setFinalScore(currentCost);
                    globalSolution.setFinalScore(globalMin);
                } else {
                    double p = (dE == 0 ? 0.5 : exp(-1.0 * dE / T));
                    double rnd = dist(mt);

                    if (rnd < p) {
                        current = tmp;
                        currentCost = newCost;
                        current.setFinalScore(currentCost);
                    }
                }

                iteration++;

#ifdef LOGARITMIC_COOLING_SCHEDULE
                T = T_0 / log(iteration + 1);
#else
                constexpr double alpha = 0.95;
                T *= alpha;
#endif
            }

            solutions[i] = new Solution(globalSolution);
            cout << "Iteration " << i << " with a minimum score of " << globalMin << "." << endl;
        }
        catch (const runtime_error &error) {
            cout << "Iteration " << i << " failed because maximum number of randomization was exceeded." << endl;
        }
    }

    int cumulativeMinimum = numeric_limits<int>::max();
    int minIndex = -1;
    for (int i = 0; i < numberOfSolutions; i++) {
        if (solutions[i]->getFinalScore() < cumulativeMinimum) {
            minIndex = i;
            cumulativeMinimum = solutions[i]->getFinalScore();
        }
    }

    writeToFile(*solutions[minIndex]);
    cout << "Minimum score [" << minIndex << "] is: " << setprecision(10) << cumulativeMinimum << endl;

    for (int i = 0; i < numberOfSolutions; i++)
        delete solutions[i];
    delete[] solutions;

    return 0;
}
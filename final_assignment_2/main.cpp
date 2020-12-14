//
// Created by jugos000 on 14-Dec-20.
//

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

constexpr int NUMBER_OF_ITERATIONS = 500000;
#ifndef LOGARITHMIC_COOLING_SCHEDULE
static_assert(NUMBER_OF_ITERATIONS > 10000,
              "Use logarithmic cooling to support number of iterations greater than 10000.");
#endif

constexpr int N = 23;
static_assert(N == 23);

#ifdef DEBUG
constexpr int DEBUG_MAX_PLACEMENT = N;
#endif

struct Point {
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}
};

struct Rectangle {
    int height;
    int width;

    Point bottomLeft;

    Rectangle(int height, int weight) : height(height), width(weight), bottomLeft(0, 0) {}

    Point getTopRight() const {
        return Point(bottomLeft.x + width, bottomLeft.y + height);
    }

    void rotate() {
        swap(width, height);
    }
};

inline void updatePlacementPoints(vector<Point> &placementPoints, const int pointToRemoveIndex, Rectangle &rectangle) {
    placementPoints.erase(placementPoints.begin() + pointToRemoveIndex);

    Point up(rectangle.bottomLeft.x, rectangle.bottomLeft.y + rectangle.height);
    Point right(rectangle.bottomLeft.x + rectangle.width, rectangle.bottomLeft.y);

    placementPoints.push_back(up);
    placementPoints.push_back(right);
}

bool isPlacementValid(vector<Rectangle> &placedRectangles, Rectangle &rectangle, Point &point) {
    Rectangle r1(rectangle);
    r1.bottomLeft = point;

    for (auto &r2 : placedRectangles) {
        const int xi1 = r1.bottomLeft.x;
        const int yi1 = r1.bottomLeft.y;
        Point topRight1 = r1.getTopRight();
        const int xi2 = topRight1.x;
        const int yi2 = topRight1.y;

        const int xj1 = r2.bottomLeft.x;
        const int yj1 = r2.bottomLeft.y;
        Point topRight2 = r2.getTopRight();
        const int xj2 = topRight2.x;
        const int yj2 = topRight2.y;

        if (!(xi1 >= xj2 || xj1 >= xi2 || yi1 >= yj2 || yj1 >= yi2))
            return false; // overlapping
    }

    return true;
}

void determineImageSize(const vector<Rectangle> &rectangles, int &sizeX, int &sizeY) {
    sizeX = numeric_limits<int>::min();
    sizeY = numeric_limits<int>::min();

    for (auto &r : rectangles) {
        if (r.getTopRight().x > sizeX)
            sizeX = r.getTopRight().x;

        if (r.getTopRight().y > sizeY)
            sizeY = r.getTopRight().y;
    }
}

inline int optimizationFunction(const vector<Rectangle> &rectangles) {
    constexpr int rectangleTotalSum = 2300;

    int sizeX, sizeY;
    determineImageSize(rectangles, sizeX, sizeY);

    return sizeX * sizeY - rectangleTotalSum;
}

int main() {
    random_device rd;
    mt19937 mt(rd());

    constexpr double T_0 = 4000.0;

    int iteration = 0;
    vector<Rectangle> bestSolution, currentSolution;
    int currentScore;
    int bestScore = numeric_limits<int>::max();

#ifdef LOGARITHMIC_COOLING_SCHEDULE
    double T = T_0 / log(2);
#else
    double T = T_0;
#endif

    while (iteration < NUMBER_OF_ITERATIONS) {
        vector<Rectangle> unplacedRectangles;
        vector<Rectangle> placedRectangles;
        vector<Point> placementPointCandidates;

        placementPointCandidates.emplace_back(0, 0);
        for (int i = 0; i < N; i++)
            unplacedRectangles.emplace_back(i + 1, 23 - i);

        int i = 0;

        while (!unplacedRectangles.empty()) {
#ifdef DEBUG
            if (i >= DEBUG_MAX_PLACEMENT)
                break;

            int indexOfRectangle;
            int indexOfCandidatePoint;

            if (i == 0) {
                indexOfRectangle = 4;
                indexOfCandidatePoint = 0;
            } else if (i == 1) {
                indexOfRectangle = 7;
                indexOfCandidatePoint = 0;
            } else if (i == 2) {
                indexOfRectangle = 9;
                indexOfCandidatePoint = 2;
            } else if (i == 3) {
                indexOfRectangle = 3;
                indexOfCandidatePoint = 1;
            } else
                throw runtime_error("Not implemented yet.");

            Rectangle &rectangle = unplacedRectangles[indexOfRectangle];
            Point &candidatePoint = placementPointCandidates[indexOfCandidatePoint];
#else
            uniform_int_distribution<int> randomRectangle(0, unplacedRectangles.size() - 1);
            uniform_int_distribution<int> randomPoint(0, placementPointCandidates.size() - 1);

            static uniform_real_distribution<double> randomRotate(0, 1);

            const int indexOfRectangle = randomRectangle(mt);
            Rectangle &rectangle = unplacedRectangles[indexOfRectangle];
            const int indexOfCandidatePoint = randomPoint(mt);
            Point &candidatePoint = placementPointCandidates[indexOfCandidatePoint];

            if (randomRotate(mt) > 0.5)
                rectangle.rotate();
#endif

            if (isPlacementValid(placedRectangles, rectangle, candidatePoint)) {
                rectangle.bottomLeft = candidatePoint;

                updatePlacementPoints(placementPointCandidates, indexOfCandidatePoint, rectangle);

                placedRectangles.push_back(rectangle);
                unplacedRectangles.erase(unplacedRectangles.begin() + indexOfRectangle);

                i++;
            }
        }

        if (iteration != 0) {
            int newScore = optimizationFunction(placedRectangles);
            int64_t dE = newScore - currentScore;
            if (dE < 0) {
                currentScore = newScore;
                currentSolution = placedRectangles;
            } else {
                static uniform_real_distribution<double> annealingGenerator(0, 1);

                if (T < 0.1) {
                    cout << "Increase initial temperature to support " << NUMBER_OF_ITERATIONS << " iterations.";
                    return 1;
                }

                double p = (dE == 0 ? 0.5 : exp(-1.0 * dE / T));
                double rnd = annealingGenerator(mt);

                if (rnd < p) {
                    currentScore = newScore;
                    currentSolution = placedRectangles;
                }
            }

#ifdef LOGARITHMIC_COOLING_SCHEDULE
            T = T_0 / log(iteration + 1);
#else
            constexpr double alpha = 0.999;
            T *= alpha;
#endif

            if (currentScore < bestScore) {
                bestSolution = currentSolution;
                bestScore = currentScore;
            }
        } else {
            currentScore = optimizationFunction(placedRectangles);
            currentSolution = placedRectangles;
        }

        iteration++;

        if (iteration % 10000 == 0)
            cout << "Progress: " << setprecision(3) << 100.0 * iteration / NUMBER_OF_ITERATIONS << "%" << endl;
    }

    fstream file;
    file.open("fa_layout.txt", fstream::out | fstream::trunc);

    int canvasX, canvasY;
    determineImageSize(bestSolution, canvasX, canvasY);
    file << canvasX << endl;
    file << canvasY << endl;
#ifdef DEBUG
    file << DEBUG_MAX_PLACEMENT;
#else
    file << N;
#endif

    for (auto &r : bestSolution)
        file << endl << r.bottomLeft.x << " " << r.bottomLeft.y << " " << r.width << " " << r.height;

    file.close();

    cout << "Best score: " << optimizationFunction(bestSolution) << endl;

    return 0;
}
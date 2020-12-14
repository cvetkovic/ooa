//
// Created by jugos000 on 14-Dec-20.
//

#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

constexpr int N = 23;
static_assert(N == 23);

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
};

void updatePlacementPoints(vector<Point> &placementPoints, const int pointToRemoveIndex, Rectangle &rectangle) {
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

void determineImageSize(vector<Rectangle> &rectangles, int &sizeX, int &sizeY) {
    sizeX = numeric_limits<int>::min();
    sizeY = numeric_limits<int>::min();

    for (auto &r : rectangles) {
        if (r.getTopRight().x > sizeX)
            sizeX = r.getTopRight().x;

        if (r.getTopRight().y > sizeY)
            sizeY = r.getTopRight().y;
    }
}

int main() {
    random_device rd;
    mt19937 mt(rd());

    fstream file;
    file.open("fa_layout.txt", fstream::out | fstream::trunc);

    vector<Rectangle> unplacedRectangles;
    vector<Rectangle> placedRectangles;
    vector<Point> placementPointCandidates;

    placementPointCandidates.push_back(Point(0, 0));
    for (int i = 0; i < N; i++)
        unplacedRectangles.push_back(Rectangle(i + 1, 23 - i));

    constexpr int DEBUG_MAX_PLACEMENT = N;
    int i = 0;

    while (!unplacedRectangles.empty()) {
        if (i >= DEBUG_MAX_PLACEMENT)
            break;

        uniform_int_distribution<int> randomRectangle(0, unplacedRectangles.size() - 1);
        uniform_int_distribution<int> randomPoint(0, placementPointCandidates.size() - 1);

#ifdef DEBUG
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
        const int indexOfRectangle = randomRectangle(mt);
        Rectangle &rectangle = unplacedRectangles[indexOfRectangle];
        const int indexOfCandidatePoint = randomPoint(mt);
        Point &candidatePoint = placementPointCandidates[indexOfCandidatePoint];
#endif

        if (isPlacementValid(placedRectangles, rectangle, candidatePoint)) {
            rectangle.bottomLeft = candidatePoint;

            updatePlacementPoints(placementPointCandidates, indexOfCandidatePoint, rectangle);

            placedRectangles.push_back(rectangle);
            unplacedRectangles.erase(unplacedRectangles.begin() + indexOfRectangle);

            i++;
        }
    }

    int canvasX, canvasY;
    determineImageSize(placedRectangles, canvasX, canvasY);
    file << canvasX << endl;
    file << canvasY << endl;
    file << DEBUG_MAX_PLACEMENT;

    for (auto &r : placedRectangles)
        file << endl << r.bottomLeft.x << " " << r.bottomLeft.y << " " << r.width << " " << r.height;

    file.close();

    return 0;
}
//
// Created by jugos000 on 26-Nov-20.
//

#ifndef IOA_RECTANGLE_HPP
#define IOA_RECTANGLE_HPP

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <unordered_map>
#include <unordered_set>

using namespace std;

constexpr int MAX_NUMBER_OF_RANDOM_GUESSES = 1000;

struct Point {
    int x;
    int y;

    Point() {}

    Point(int x, int y) : x(x), y(y) {}

    friend ostream &operator<<(ostream &os, const Point &shape) {
        os << shape.x << " " << shape.y;
        return os;
    }
};

class Rectangle {

protected:
    int width;
    int height;

public:
    Point bottomLeft;

    Rectangle(Point bottomLeft, const int width, const int height) :
            bottomLeft(bottomLeft), width(width), height(height) {}

    void rotate() { swap(width, height); }

    int getWidth() const { return width; }

    int getHeight() const { return height; }

    int calculateArea() const { return width * height; }

    Point getTopLeft() const;

    Point getTopRight() const;

    Point getBottomLeft() const { return bottomLeft; }

    Point getBottomRight() const;

    friend ostream &operator<<(ostream &os, const Rectangle &rectangle) {
        Point upperLeft = rectangle.getTopLeft();

        os << upperLeft << " " << rectangle.width << " " << rectangle.height;

        return os;
    }

    static bool overlap(const Rectangle &r1, const Rectangle &r2) {
        const int xi1 = r1.bottomLeft.x;
        const int yi1 = -r1.bottomLeft.y;
        Point topRight1 = r1.getTopRight();
        const int xi2 = topRight1.x;
        const int yi2 = -topRight1.y;

        const int xj1 = r2.bottomLeft.x;
        const int yj1 = -r2.bottomLeft.y;
        Point topRight2 = r2.getTopRight();
        const int xj2 = topRight2.x;
        const int yj2 = -topRight2.y;

        if (xi1 >= xj2 || xj1 >= xi2 || yi1 >= yj2 || yj1 >= yi2)
            return false; // no overlapping
        else
            return true; // overlapping
    }

    friend class Solution;

    static bool outsideOfCanvas(const Rectangle &rectangle);

    Rectangle() {

    }

    void setPosition(int x, int y, bool rotate);
};

class Solution {

protected:
    const int N;
    Rectangle *rectangles;

    static random_device rd;
    static mt19937 mt;
    static uniform_int_distribution<int> dist;

    int canvasSizeX, canvasSizeY;
    double finalScore = numeric_limits<double>::max();

public:
    Solution(const int N) : N(N) {
        rectangles = new Rectangle[N];
    }

    Solution(const Solution &copyFrom) : N(copyFrom.N) {
        rectangles = new Rectangle[N];
        for (int i = 0; i < N; i++)
            rectangles[i] = copyFrom.rectangles[i];

        this->canvasSizeX = copyFrom.canvasSizeX;
        this->canvasSizeY = copyFrom.canvasSizeY;
        this->finalScore = copyFrom.finalScore;
    }

    ~Solution() { delete[] rectangles; }

    const Rectangle *getRectangles() const { return rectangles; }

    int getCanvasSizeX() const { return canvasSizeX; }

    int getCanvasSizeY() const { return canvasSizeY; }

    void hamming(const int distance);

    void generateRandomLayout();

    Solution operator=(const Solution &from) {
        Solution to(from.N);
        for (int i = 0; i < from.N; i++)
            to.rectangles[i] = from.rectangles[i];
        to.finalScore = from.finalScore;
        to.canvasSizeX = from.canvasSizeX;
        to.canvasSizeY = from.canvasSizeY;

        return to;
    }

    double getFinalScore() const {
        return finalScore;
    }

    void setFinalScore(double score) {
        finalScore = score;
    }

};

#endif //IOA_RECTANGLE_HPP

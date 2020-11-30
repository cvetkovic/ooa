//
// Created by jugos000 on 26-Nov-20.
//

#ifndef IOA_RECTANGLE_HPP
#define IOA_RECTANGLE_HPP

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>

using namespace std;

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
    Point bottomLeft;
    int width;
    int height;

    Rectangle() {

    }

public:
    Rectangle(Point bottomLeft, const int width, const int height) :
            bottomLeft(bottomLeft), width(width), height(height) {}

    void rotate() { swap(width, height); }

    int getWidth() const { return width; }

    int getHeight() const { return height; }

    int calculateArea() { return width * height; }

    Point getTopRight() const;

    Point getBottomLeft() { return bottomLeft; }

    friend ostream &operator<<(ostream &os, const Rectangle &rectangle) {
        os << rectangle.bottomLeft << " " << rectangle.width << " " << rectangle.height;

        return os;
    }

    static bool overlap(const Rectangle &r1, const Rectangle &r2) {
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

        if (xi1 >= xj2 || xj1 >= xi2 || yi1 >= yj2 || yj1 >= yi2)
            return false; // no overlapping
        else
            return true; // overlapping
    }

    friend class Solution;

    static bool outsideOfCanvas(const Rectangle &rectangle);
};

class Solution {

protected:
    const int N;
    Rectangle *rectangles;

    static random_device rd;
    static mt19937 mt;
    static uniform_int_distribution<int> dist;

    void generateRandomLayout();

    int canvasSizeX, canvasSizeY;

public:
    Solution(const int N) : N(N) {
        rectangles = new Rectangle[N];
        generateRandomLayout();
    }

    ~Solution() { delete rectangles; }

    const Rectangle *getRectangles() const { return rectangles; }

    int getCanvasSizeX() const { return canvasSizeX; }

    int getCanvasSizeY() const { return canvasSizeY; }

};

#endif //IOA_RECTANGLE_HPP
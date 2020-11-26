//
// Created by jugos000 on 26-Nov-20.
//

#ifndef IOA_RECTANGLE_HPP
#define IOA_RECTANGLE_HPP

#include <iostream>

using namespace std;

struct Point {
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}

    friend ostream &operator<<(ostream &os, const Point &shape) {
        os << shape.x << " " << shape.y;
        return os;
    }
};

class Shape {

protected:
    Point upperLeft;

    explicit Shape(Point upperLeft) : upperLeft(upperLeft) {}

    virtual ~Shape() = default;

public:

    virtual void rotateLeft() = 0;

    virtual void rotateRight() = 0;

    virtual void flipHorizontally() = 0;

    virtual void flipVertically() = 0;

    Point &getUpperLeft() {
        return upperLeft;
    }

};

class Rectangle : public Shape {

protected:
    int width;
    int height;

public:
    Rectangle(Point upperLeft, const int width, const int height) :
            Shape(upperLeft), width(width), height(height) {}

    void rotateLeft() override;

    void rotateRight() override;

    void flipHorizontally() override;

    void flipVertically() override;

    int getWidth() const { return width; }

    int getHeight() const { return height; }

    friend ostream &operator<<(ostream &os, const Rectangle &rectangle) {
        os << rectangle.upperLeft << " " << rectangle.width << " " << rectangle.height;

        return os;
    }

};


#endif //IOA_RECTANGLE_HPP

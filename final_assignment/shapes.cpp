//
// Created by jugos000 on 26-Nov-20.
//

#include "shapes.hpp"

Point Rectangle::getTopRight() const {
    return Point(bottomLeft.x + width, bottomLeft.y - height);
}

bool Rectangle::outsideOfCanvas(const Rectangle &rectangle) {
    Point topRight = rectangle.getTopRight();

    if (topRight.x < 0 || topRight.y < 0)
        return true;
    else
        return false;
}

// TODO: check for uniformity
random_device Solution::rd;
mt19937 Solution::mt(rd());
uniform_int_distribution<int> Solution::dist(0, 276);

void Solution::generateRandomLayout() {
    // set width and height
    for (int i = 1; i <= 23; i++) {
        int width = i;
        int height = 24 - i;

        rectangles[i - 1].width = width;
        rectangles[i - 1].height = height;
    }

    // generate bottom corner
    for (int i = 0; i < N; i++) {
        while (true) {
            int x = dist(mt);
            int y = dist(mt);

            rectangles[i].bottomLeft.x = x;
            rectangles[i].bottomLeft.y = y;

            if (dist(mt) % 2 == 1)
                rectangles[i].rotate();

            if (Rectangle::outsideOfCanvas(rectangles[i]))
                continue;

            ////////////////////////////////
            // check for constraints
            ////////////////////////////////
            bool exitLoop = true;
            for (int j = 0; j < i; j++) {
                Rectangle &r1 = rectangles[j];
                Rectangle &r2 = rectangles[i];

                if (&r1 == &r2)
                    continue;

                if (Rectangle::overlap(r1, r2))
                    exitLoop = false;
            }

            // continue if constraints not met
            if (exitLoop)
                break;
        }
    }

    ////////////////////////////////
    // determine canvas size
    ////////////////////////////////
    int minX = numeric_limits<int>::max();
    int minY = numeric_limits<int>::max();
    int maxX = numeric_limits<int>::min();
    int maxY = numeric_limits<int>::min();
    for (int i = 0; i < N; i++) {
        Point &bottomLeft = rectangles[i].bottomLeft;
        Point topRight = rectangles[i].getTopRight();

        if (bottomLeft.x < minX)
            minX = bottomLeft.x;
        if (bottomLeft.y > maxY)
            maxY = bottomLeft.y;

        if (topRight.x > maxX)
            maxX = topRight.x;
        if (topRight.y < minY)
            minY = topRight.y;
    }

    canvasSizeX = maxX - minX;
    canvasSizeY = maxY - minY;

    ////////////////////////////////
    // shift everything towards northwest
    ////////////////////////////////
    for (int i = 0; i < N; i++) {
        rectangles[i].bottomLeft.x -= minX;
        rectangles[i].bottomLeft.y -= minY;
    }
}

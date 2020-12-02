//
// Created by jugos000 on 26-Nov-20.
//

#include "shapes.hpp"

Point Rectangle::getTopLeft() const {
    return Point(bottomLeft.x, bottomLeft.y - height);
}

Point Rectangle::getTopRight() const {
    return Point(bottomLeft.x + width, bottomLeft.y - height);
}

Point Rectangle::getBottomRight() const {
    return Point(bottomLeft.x + width, bottomLeft.y);
}

bool Rectangle::outsideOfCanvas(const Rectangle &rectangle) {
    Point topRight = rectangle.getTopRight();

    if (topRight.x < 0 || topRight.y < 0)
        return true;
    else
        return false;
}

random_device Solution::rd;
mt19937 Solution::mt(rd());
uniform_int_distribution<int> Solution::dist(0, 60);

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
        int numberOfTries = 0;

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
                Rectangle &r1 = rectangles[i];
                Rectangle &r2 = rectangles[j];

                if (&r1 == &r2)
                    continue;

                if (Rectangle::overlap(r1, r2))
                    exitLoop = false;
            }

            if (++numberOfTries > MAX_NUMBER_OF_RANDOM_GUESSES)
                throw runtime_error("Exceeded maximum number of randomisation.");

            // continue if constraints are not met
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

void Solution::hamming(const int distance) {
    unordered_map<int, int> rectangleIndexMap;
    int i = 0;

    static uniform_int_distribution<int> dist(0, 22);
    static uniform_int_distribution<int> bitPosition(0, 5);

    // determine which rectangles to change
    while (i < distance) {
        int num = dist(mt);

        if (rectangleIndexMap[num] <= 5) {
            rectangleIndexMap[num]++;
            i++;
        }
    }

    for (i = 0; i < N; i++) {
        int cnt = rectangleIndexMap[i];
        unordered_set<int> bits;

        while (cnt > 0) {
            // generate position of random bit to flip
            if (dist(mt) % 2 != 0) {
                int position;
                int numberOfTries = 0;
                do {
                    position = bitPosition(mt);
                    if (bits.find(position) == bits.end())
                        break;
                    if (++numberOfTries > MAX_NUMBER_OF_RANDOM_GUESSES)
                        throw runtime_error("Exceeded maximum number of randomisation.");
                } while (true);

                // flip the bit
                if (dist(mt) % 2 == 0)
                    rectangles[i].bottomLeft.x ^= (1 << position);
                else
                    rectangles[i].bottomLeft.y ^= (1 << position);
            } else
                rectangles[i].rotate();

            // check for constraints
            if (Rectangle::outsideOfCanvas(rectangles[i]))
                continue;

            ////////////////////////////////
            // check for constraints
            ////////////////////////////////
            bool decrementCnt = true;
            for (int j = 0; j < N; j++) {
                Rectangle &r1 = rectangles[i];
                Rectangle &r2 = rectangles[j];

                if (&r1 == &r2)
                    continue;

                if (Rectangle::overlap(r1, r2)) {
                    decrementCnt = false;
                    break;
                }
            }

            if (decrementCnt)
                cnt--;
        }
    }
}

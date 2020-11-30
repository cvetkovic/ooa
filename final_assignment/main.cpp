//
// Created by jugos000 on 26-Nov-20.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "shapes.hpp"

using namespace std;

constexpr int N = 23;

// 1 + 2 + ... 23 = 276

inline int optimizationFunction(vector<Rectangle> &shapes, int canvasArea) {
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

    Solution initial(N);

    writeToFile(initial);

    return 0;
}
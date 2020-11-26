//
// Created by jugos000 on 26-Nov-20.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "shapes.hpp"

using namespace std;

constexpr int N = 23;

vector<Rectangle> *createShapes() {
    vector<Rectangle> *rectangles = new vector<Rectangle>();
    Point initialPoint(0, 0);

    for (int i = 1; i <= 23; i++) {
        int width = i;
        int height = 24 - i;

        rectangles->push_back(Rectangle(initialPoint, width, height));
    }

    return rectangles;
}

inline int optimizationFunction(vector<Rectangle> &shapes, int canvasArea) {
    int result = canvasArea;

    for (auto &rectangle : shapes)
        result -= rectangle.calculateArea();

    return result;
}

void optimize(const vector<Rectangle> &shapes) {

}

void writeToFile(const vector<Rectangle> &shapes, int canvasSizeX, int canvasSizeY) {
    ofstream file("fa_layout.txt");

    file << canvasSizeX << endl;
    file << canvasSizeY << endl;

    for (const Rectangle &rectangle : shapes)
        file << rectangle << endl;

    file.close();
}

int main(int argc, char **argv) {
    vector<Rectangle> *rectangles = createShapes();

    writeToFile(*rectangles, 800, 600);

    delete rectangles;

    return 0;
}
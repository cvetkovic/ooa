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

void optimize(const vector<Rectangle> &shapes) {

}

void writeToFile(const vector<Rectangle> &shapes) {
    ofstream file("fa_layout.txt");

    for (const Rectangle &rectangle : shapes)
        file << rectangle << endl;

    file.close();
}

int main(int argc, char **argv) {
    vector<Rectangle> *rectangles = createShapes();

    writeToFile(*rectangles);

    delete rectangles;

    return 0;
}
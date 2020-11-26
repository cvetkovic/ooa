//
// Created by jugos000 on 26-Nov-20.
//

#include "shapes.hpp"

void Rectangle::rotateLeft() {
    swap(width, height);
}

void Rectangle::rotateRight() {
    swap(width, height);
}

void Rectangle::flipHorizontally() {
    throw runtime_error("Not yet implemented.");
}

void Rectangle::flipVertically() {
    throw runtime_error("Not yet implemented.");
}
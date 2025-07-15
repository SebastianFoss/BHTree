//
// Created by sailsec on 7/7/25.
//

#ifndef BOX_H
#define BOX_H
#include <array>

#include "Vec.h"
#include "array"

// a simple coordinate system devised to track the bounds of a Node

struct Box {
    Vec min;
    Vec max;

    Box(Vec min, Vec max) : min(min), max(max) {}
    Box() : min(Vec()),max(Vec()) {} // zero initialized


    // Box[] subdivide() {} // maybe?
    double getWidth() const {}
    double getHalfWidth() const {}

    bool contains(Vec p) const {}

};



#endif //BOX_H

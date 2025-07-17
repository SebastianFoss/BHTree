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


    // getters
    Vec getCenter() const {
        return (min + max) * 0.5;
    }
    double getWidth() const {
        return max.x - min.x;
    }
    double getHalfWidth() const {
        return getWidth() / 2;
    }
    bool contains(const Vec& point) const {
        return point.x >= min.x && point.x <= max.x &&
            point.y >= min.y && point.y <= max.y &&
            point.z >= min.z && point.z <= max.z;
    }

    // returns the index (0-7) of a child's quadrant/octant
    // that a given particle's position is in.
    int getOctantIndex(const Vec& point) const {
        int index = 0;
        Vec center = getCenter();

        // decides the bits for coordinate components
        if (point.x > center.x) index |= 4;
        if (point.y > center.y) index |= 2;
        if (point.z > center.z) index |= 1;

        return index;
    }

    std::array<Box, 8> subdivide() const {
        std::array<Box, 8> children;
        Vec center = getCenter();
        double halfX = (max.x - min.x) / 2;
        double halfY = (max.y - min.y) / 2;
        double halfZ = (max.z - min.z) / 2;

       // Lower bottom left
        // lower bottom
    }

};



#endif //BOX_H

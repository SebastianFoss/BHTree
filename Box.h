//
// Created by sailsec on 7/7/25.
//

#ifndef BOX_H
#define BOX_H
#include <array>
#include "Vec.h"

// A simple coordinate system devised to track the bounds of a Node (Axis-Aligned Bounding Box)
struct Box {
    Vec min; // Minimum coordinates (x, y, z)
    Vec max; // Maximum coordinates (x, y, z)

    // Constructors
    Box(const Vec& min_coords, const Vec& max_coords) : min(min_coords), max(max_coords) {}
    Box() : min(Vec()), max(Vec()) {} // Default constructor: zero-initialized AABB at origin

    // Getters
    Vec getCenter() const {
        return (min + max) * 0.5;
    }

    // Assumes a cubic box for octree. Returns the length of one side.
    double getSideLength() const {
        // All dimensions should be equal for a perfectly cubic octree box
        // if not, using max.x - min.x is a valid choice, but the node
        // might not be perfectly cubic, which can lead to non-uniform subdivisions.
        return max.x - min.x;
    }

    // Checks if a given point is contained within this bounding box
    bool contains(const Vec& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }

    // Returns the index (0-7) of the child octant that a given particle's position falls into.
    // Indexing convention:
    // Bit 2 (MSB): X-axis (0 if point.x <= center.x, 1 if point.x > center.x)
    // Bit 1: Y-axis (0 if point.y <= center.y, 1 if point.y > center.y)
    // Bit 0 (LSB): Z-axis (0 if point.z <= center.z, 1 if point.z > center.z)
    //
    // Example: Octant 0 (000) = (min.x-center.x, min.y-center.y, min.z-center.z)
    // Example: Octant 7 (111) = (center.x-max.x, center.y-max.y, center.z-max.z)
    int getOctantIndex(const Vec& point) const {
        int index = 0;
        Vec center = getCenter();

        if (point.x > center.x) index |= 4; // Set bit 2 (X-axis)
        if (point.y > center.y) index |= 2; // Set bit 1 (Y-axis)
        if (point.z > center.z) index |= 1; // Set bit 0 (Z-axis)

        return index;
    }

    // Subdivides the current box into 8 smaller, equally sized child octant boxes.
    std::array<Box, 8> subdivide() const {
        std::array<Box, 8> children_boxes;
        Vec center = getCenter();

        // The 8 octants are defined by combining min/center/max coordinates for each axis
        // Children are ordered by the getOctantIndex convention (Z-Y-X bit order)

        // Octant 0 (000: -- min X, min Y, min Z)
        children_boxes[0] = Box(Vec(min.x, min.y, min.z),
                                Vec(center.x, center.y, center.z));
        // Octant 1 (001: -- min X, min Y, max Z)
        children_boxes[1] = Box(Vec(min.x, min.y, center.z),
                                Vec(center.x, center.y, max.z));
        // Octant 2 (010: -- min X, max Y, min Z)
        children_boxes[2] = Box(Vec(min.x, center.y, min.z),
                                Vec(center.x, max.y, center.z));
        // Octant 3 (011: -- min X, max Y, max Z)
        children_boxes[3] = Box(Vec(min.x, center.y, center.z),
                                Vec(center.x, max.y, max.z));
        // Octant 4 (100: -- max X, min Y, min Z)
        children_boxes[4] = Box(Vec(center.x, min.y, min.z),
                                Vec(max.x, center.y, center.z));
        // Octant 5 (101: -- max X, min Y, max Z)
        children_boxes[5] = Box(Vec(center.x, min.y, center.z),
                                Vec(max.x, center.y, max.z));
        // Octant 6 (110: -- max X, max Y, min Z)
        children_boxes[6] = Box(Vec(center.x, center.y, min.z),
                                Vec(max.x, max.y, center.z));
        // Octant 7 (111: -- max X, max Y, max Z)
        children_boxes[7] = Box(Vec(center.x, center.y, center.z),
                                Vec(max.x, max.y, max.z));

        return children_boxes;
    }
};

#endif //BOX_H
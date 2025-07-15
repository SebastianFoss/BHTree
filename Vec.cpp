//
// Created by sailsec on 7/7/25.
//

#include "Vec.h"

#include <stdexcept>

// Vec operator+(const Vec& rhs) const;
// Vec operator-(const Vec& rhs) const;
// Vec operator*(double rhs) const;
// Vec operator/(double rhs) const;

Vec Vec::operator+(const Vec& rhs) const {
    return Vec(x+rhs.x, y+rhs.y, z+rhs.z);
};
Vec Vec::operator-(const Vec& rhs) const {
    return Vec(x-rhs.x, y-rhs.y, z-rhs.z);
};

Vec Vec::operator*(double rhs) const {
    return Vec(x*rhs, y*rhs, z*rhs);
};
Vec Vec::operator/(double rhs) const {
    if (rhs == 0.0) {
        throw std::invalid_argument("Division by 0");
    }
    return Vec(x/rhs, y/rhs, z/rhs);
};
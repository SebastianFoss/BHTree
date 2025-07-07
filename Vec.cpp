//
// Created by sailsec on 7/7/25.
//

#include "Vec.h"

#include <stdexcept>

// Vec operator+(const Vec& rhs) const;
// Vec operator-(const Vec& rhs) const;
// Vec operator*(double rhs) const;
// Vec operator/(double rhs) const;

Vec::Vec operator+(const Vec& rhs) {

};
Vec::Vec operator-(const Vec& rhs) {

};

Vec::Vec operator*(double rhs) {

};
Vec::Vec operator/(double rhs) {
    if (rhs == 0.0) {
        throw std::invalid_argument("Division by 0");
    }
};
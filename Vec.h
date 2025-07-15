//
// Created by sailsec on 7/7/25.
//

#ifndef VEC_H
#define VEC_H

// 3d Vector struct
// instantiated explicitly in: particle
// stack allocated
// only allow explicit declaration

struct Vec {
    double x, y, z;
    Vec(double x, double y, double z) : x(x), y(y), z(z) {}
    Vec() : x(0), y(0), z(0) {}




    double magnitude() const;
    void dot(Vec &other) const;
    void normalize();

    // vector operations

    Vec operator+(const Vec& rhs) const;
    Vec operator-(const Vec& rhs) const;
    Vec operator*(double rhs) const;
    Vec operator/(double rhs) const;

};



#endif //VEC_H

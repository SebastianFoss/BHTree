//
// Created by sailsec on 7/7/25.
//

#ifndef VEC_H
#define VEC_H

// 3d Vector struct
// instantiated explicitly in: particle
// stack allocated

struct Vec {
    double x, y, z;
    Vec(double x, double y, double z) : x(x), y(y), z(z) {}
    Vec() : x(0), y(0), z(0) {}
};



#endif //VEC_H

//
// Created by sailsec on 7/7/25.
//

#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vec.h"

// only allow explicit declaration


// Particle as an Array

// Nx7 array
class Particle {
private:
    Vec pos;
    Vec vel;
    Vec acc;
    double mass;
    int id;

public:
    Particle(Vec pos, double mass) : mass(mass) {
        // pos = new Vec(pos);
    }
    // will be allocated with std::make_unique, on heap

    // getters
    Vec getPos() const;
    Vec getVel() const;
    Vec getAcc() const;
    double getMass() const;
};



#endif //PARTICLE_H

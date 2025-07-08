//
// Created by sailsec on 7/7/25.
//

#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vec.h"

// only allow explicit declaration

class Particle {
private:
    Vec* pos;
    double mass;

public:
    Particle(Vec pos, double mass) : mass(mass) {
        this->pos = new Vec(pos);
    }
    // will be allocated with std::make_unique, on heap

    // getters
    Vec getPos();
    double getMass();

    ~Particle() {
        delete pos;
    }
};



#endif //PARTICLE_H

//
// Created by sailsec on 7/7/25.
//

#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vec.h"

// only allow explicit declaration

class Particle {
private:
    Vec pos;
    double mass;

public:
    Particle(Vec pos, double mass) : pos(pos), mass(mass) {}
    // will be allocated with std::make_unique, on heap


    // time step components
    void updatePos(double dt);
    void updateVel(double dt);
};



#endif //PARTICLE_H

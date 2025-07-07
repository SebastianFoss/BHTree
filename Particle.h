//
// Created by sailsec on 7/7/25.
//

#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vec.h"


class Particle {
private:
    Vec pos;
    double mass;

public:
    Particle(Vec pos, double mass) : pos(pos), mass(0) {}

};



#endif //PARTICLE_H

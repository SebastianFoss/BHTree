//
// Created by sailsec on 7/7/25.
//

#ifndef BOX_H
#define BOX_H
#include "Vec.h"

// a simple coordinate system devised to track the bounds of a Node

struct Box {

    double width;
    Vec min, max;

    Box(Vec min, Vec max) : min(min), max(max) {
        width = max.x - min.x;
    }
};



#endif //BOX_H

//
// Created by sailsec on 7/7/25.
//

#ifndef BHTREE_H
#define BHTREE_H

#include <vector>
#include <memory>

#include "Node.h"
#include "NodePool.h"
#include "Particle.h"


class BHtree {

private:
    // the list of initial particle pointers as input
    std::vector<std::unique_ptr<Particle>> particles;
    // the pointers own the particles fully

    // the initial node which will recursively expand the tree
    Node* root;

    // the pool of all free nodes
    NodePool nodePool;

    Box box;

public:

    // initializes node_pool
    // runs particles.reserve
    //
    BHtree(std::vector<Particle> initial_particles();

    void calculateBounds() {

        for (int i = 0; i < particles.size(); i++) {

        }
    }



};



#endif //BHTREE_H

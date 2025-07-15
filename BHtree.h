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

    // int max_depth = number of generations / number of branches
    // int max_particles = the max number of particles?
    //


    // the list of initial particle pointers as input
    std::vector<std::unique_ptr<Particle>> particles;
    // the pointers own the particles fully

    // the initial node which will recursively expand the tree
    // BHtree owns the root, which means must be unique
    std::unique_ptr<Node> root;

    // the pool of all free nodes
    // a pointer for strong ownership, and no need to construct initially
    std::unique_ptr<NodePool> nodePool;

    // the external bounds for the tree represented by Box struct
    // stored as a value, no pointer needed
    Box box;
    // box has width, and corners represented by vecs


    // pre: all particles must be valid
    // post: BHtree box (bounds) set to min and max coords
    void calculateBounds() {
        if (particles.empty()) {
            throw std::invalid_argument("BHtree::calculateBounds: particles is empty");
        }

        // dx = max(x) - min(x)
        // dy = max(y) - min(y)
        // dz = max(z) - min(z)

        //  L = max ( dx, dy, dz ), + the padding

        // Center = Vec(Cx, Cy, Cz)


        // Cx = (Max x + min x) / 2
        // Cy = ...
    }




    // Children indeces vectorized -> associates particles with a location, and then associates them to a child node
    // have to check the coordinates to the quarter widths to verify their locations
    // quad: 0123, top left to br
    // Particles each have coordinates
    //


public:


    // Primary constructor
    // initializes node_pool
    // runs particles.reserve
    BHtree(std::vector<Particle> initial_particles())
        // we reserve N * 2 nodes in memory, where N is num of particles
        : nodePool(std::make_unique<NodePool>(initial_particles().size()*2)), box(Vec(0,0,0), Vec(0,0,0) {

        // sets the pointer array to the length of the particles array
        particles.reserve(initial_particles().size());

        // converts the inputted array of particles into the array of pointers
        for (const auto& particle: initial_particles()) {
            particles.push_back(std::make_unique<Particle>(particle));
        }

        // calculates the external-most bounds for the bounding box
        calculateBounds();
    }

};



#endif //BHTREE_H

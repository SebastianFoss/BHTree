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
        double maxi = 0, maxj = 0, maxk = 0;
        double mini = 0, minj = 0, mink= 0;
        for (int i = 0; i < particles.size(); i++) {
            if (particles[i]->getPos().x > maxi) {
                maxi = particles[i]->getPos().x;
            }
            if (particles[i]->getPos().y > maxj) {
                maxj = particles[i]->getPos().y;
            }
            if (particles[i]->getPos().z > maxk) {
                maxk = particles[i]->getPos().z;
            }
            if (particles[i]->getPos().x < mini) {
                mini = particles[i]->getPos().x;
            }
            if (particles[i]->getPos().y < minj) {
                minj = particles[i]->getPos().y;
            }
            if (particles[i]->getPos().z < mink) {
                mink = particles[i]->getPos().z;
            }

            Vec lowerBounds(mini, minj, mink );
            Vec upperBounds(maxi, maxj, maxk);

            box = Box(lowerBounds, upperBounds);
        }
    }

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

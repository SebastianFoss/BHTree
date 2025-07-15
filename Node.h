//
// Created by sailsec on 7/7/25.
//

#ifndef NODE_H
#define NODE_H
#include "Box.h"
#include "Vec.h"
#include <array>
#include <vector>

#include "NodePool.h"
#include "Particle.h"

// class invariants yet to be added

class Node {

private:
    Box box;

    double totalMass;
    Vec centerOfMass;



    // if one or no nodes
    Particle* particle;

    // used to keep track of initial particles for construction
    std::vector<Particle*> particles_in_node_for_build;
    // holds 0/1 particles for leafs, more for internals.
    // checked by checker method

    bool isLeaf;

public:

    // initializes Node with designated bounds as Box struct
    Node(Box box)
        // initialze every member to zero -- all are contingent on adding particles
        : box(box),
        isLeaf(true), // a new node starts as a potential leaf, and only becomes internal when subdivision is necessary
        totalMass(0) {
        for (Node*& child: children) {
            child = nullptr;
        }
        // particles_in_node_for_build is null by default.
    };

    // for when a Node is reused from the pool
    // resets leaf status, clears particles, sets m to 0, deallocates arr
    // clears any state from its previous use in the tree.
    void reset() {

        // sets vector values to zero.
        centerOfMass.reset();

        // reset members
        totalMass = 0;
        isLeaf = true;

        // CRUCIALLY: reset particles in node for build to zero. This ensures the particles are not re-initialized.
        particles_in_node_for_build.clear();

        // set all children node ptrs to nullptr
        for (Node*& child: children) {
            child = nullptr;
        }
    };


    // add a particle to the Node
    // called in recursive function?
    // move semantics?
    // pre: assumes particle has a valid state
    void addParticle(Particle* particle) {
        // simply push it to the array
        particles_in_node_for_build.push_back(particle);
    };

    // the recursive function
    // calls nodepool acquire 8 times to get child nodes
    // calls children[i]->addParticle(...) to distribute the particles
    // recursively calls itself (buildTree) on children[i]
    // clears particles_in_node_for_build for current node if it becomes internal.
    void buildTree(const std::vector<Particle*>& allParticles, NodePool& pool) {

        // checks node state on BHTree build

        // TERMINATION CASES
        // zero particles case
        // check if node has no particles
        // is a leaf
        if (particles_in_node_for_build.empty()) {
            isLeaf = true;
            totalMass = 0;
            // zero vector
        }
        // single particle case
        //
        if (particles_in_node_for_build.size() == 1) {
            isLeaf = true;
            // calculate center of mass
            return;
        }

        // Max depth conditional if necessary

        // END TERMINATION CASES

        // SUBDIVISION CASE
        if (particles_in_node_for_build.size() > 1) {
            isLeaf = false;

            return;
        }
    };

    // Calculates Node CoM and total mass based on children's properties.
    // only if leaf?
    void calculateCenterOfMassFromChildren();


};



#endif //NODE_H

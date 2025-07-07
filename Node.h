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

class Node {

private:
    Box box;
    Vec centerOfMass;
    double totalMass;

    // unallocated children
    std::array<Node*, 8> children;

    // used to keep track of initial particles for construction
    std::vector<Particle*> particles_in_node_for_build;
    // holds 0/1 particles for leafs, more for internals.
    // checked by checker method

    bool isLeaf;

public:

    // initializes Node with designated bounds as Box struct
    Node(Box box);

    // for when a Node is reused from the pool
    // resets leaf status, clears particles, sets m to 0, deallocates arr
    void reset();


    // add a particle to the Node
    // called in recursive function?
    // move semantics?
    void addParticle(Particle* p);


    // recursive function
    // calls nodepool acquire 8 times to get child nodes
    // calls children[i]->addParticle(...) to distribute the particles
    // recursively calls itself (buildTree) on children[i]
    // clears particles_in_node_for_build for current node if it becomes internal.
    void buildTree(const std::vector<Particle*>& allParticles, NodePool& pool);

    // Calculates Node CoM and total mass based on children's properties.
    // only if leaf?
    void calculateCenterOfMassFromChildren();


};



#endif //NODE_H

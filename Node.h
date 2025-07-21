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


class NodePool;

// class invariants yet to be added

class Node {

private:
    Box box;
    double totalMass;
    Vec centerOfMass;
    Particle* particle; // points to a single particle if it is a leaf
    std::array<Node*, 8> children; // Nodepool-managed children pointers


    // called only by NodePool
    // initializes Node with designated bounds as Box struct
    Node(const Box& box)
        // initialze every member to zero -- all are contingent on adding particles
        : box(box),
        totalMass(0), // a new node starts as a potential leaf, and only becomes internal when subdivision is necessary
        particle(nullptr) {
        for (Node*& child: children) {
            child = nullptr;
        }
        // particles_in_node_for_build is null by default.
    };

    friend class NodePool; // allows NodePool to do so

public:

    // getters
    bool isLeaf() const {
        return particle != nullptr;
    }
    bool isEmpty() const {
        if (particle != nullptr) {
            return false;
        }
        for (const Node* child: children) {
            if (child != nullptr) {
                return false;
            }
        }
        return true;
    }
    const Box& getBox() {
        return box;
    }
    const double getTotalMass() const {
        return totalMass;
    }
    const Vec& getCenterOfMass() const {
        return centerOfMass;
    }
    Node* getChild(int index) const {
        return children[index];
    }
    const Particle* getParticle() const {
        return particle;
    }

    // for when a Node is reused from the pool
    // resets leaf status, clears particles, sets m to 0, deallocates arr
    // clears any state from its previous use in the tree.
    void reset(const Box& box) {
        this->box = box;
        totalMass = 0;
        particle = nullptr;
        centerOfMass = Vec();
        for (Node*& child: children) {
            child = nullptr;
        }
    };

    void updateMassAndCenterOfMass(const Particle* p) {
        if (totalMass == 0.0) {
            centerOfMass = p->getPos();
            totalMass = p->getMass();
        }
        else {
            centerOfMass = (centerOfMass * totalMass) + (p->getPos() * p->getMass()) / (totalMass + p->getMass());
        }
    }

    void addParticle(Particle* newParticle, NodePool& pool) {
        // update Node's total mass and center of mass
        updateMassAndCenterOfMass(newParticle);

        if (isLeaf()) { // if the current node contains no or one particles
            if (particle == nullptr) {
                particle = newParticle;
                // the node is currently empty so it becomes leaf
                // now holds this particle as primary
            }
            else {
                // the node contains a particle
                // 1. move existing particle to appropriate child node
                Particle* oldParticle = particle;
                particle = nullptr;
                // this particle is no longer a leaf

                // 2. subdivide the box into 8 octants
                std::array<Box, 8> childBoxes = box.subdivide();

                // 3. Get and populate 8 children nodes from pool for existing particle
                int oldIndex = box.getOctantIndex(oldParticle->getPos());
                if (children[oldIndex] == nullptr) {
                    children[oldIndex] = pool.acquireNode(children); // allocates a node from pool
                }

                // 4. Insert the new particle into the appropriate child
                int newID = box.getOctantIndex(newParticle->getPos());
                if (children[newID] == nullptr) {
                    children[newID] = pool.acquireNode(children)
                }

            }
        }

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

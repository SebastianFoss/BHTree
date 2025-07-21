//
// Created by sailsec on 7/7/25.
//

#ifndef NODE_H
#define NODE_H
#include "Box.h"
#include "Vec.h"
#include <array>
#include <limits>
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

    // would like to allow for an externally defined G at some point


    // called only by NodePool and therefore private by friend class
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

    void updateMassAndCenterOfMass(const Particle* p) {
        if (totalMass == 0.0) {
            centerOfMass = p->getPos();
            totalMass = p->getMass();
        }
        else {
            centerOfMass = ((centerOfMass * totalMass) + (p->getPos() * p->getMass())) / (totalMass + p->getMass());
        }
    }

public:

    // checkers
    bool allChildrenNull() const {
        for (const Node* child : children) {
            if (child != nullptr) {
                return false;
            }
        }
        return true;
    }
    bool isLeaf() const {
        return particle != nullptr || allChildrenNull();
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

    // getters
    const Box& getBox() const {
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

    void addParticle(Particle* newParticle, NodePool& pool) {
        updateMassAndCenterOfMass(newParticle); // Always update this node's aggregate

        if (isEmpty()) { // This node is currently empty
            particle = newParticle; // It becomes a leaf holding this particle
        }

        else if (particle != nullptr) {
            // This node is a leaf holding one particle, now needs to subdivide
            Particle* oldParticle = particle; // save the current particle
            particle = nullptr; // This node is no longer a leaf holding a particle directly

            std::array<Box, 8> childBoxes = box.subdivide();

            // Recursively insert the existing particle
            int oldIndex = box.getOctantIndex(oldParticle->getPos());
            if (children[oldIndex] == nullptr) {
                children[oldIndex] = pool.acquireNode(childBoxes[oldIndex]); // Correct parameter
            }
            children[oldIndex]->addParticle(oldParticle, pool); // RECURSIVE CALL

            // Recursively insert the new particle
            int newIndex = box.getOctantIndex(newParticle->getPos()); // Corrected variable name from newID
            if (children[newIndex] == nullptr) {
                children[newIndex] = pool.acquireNode(childBoxes[newIndex]); // Correct parameter
            }

            children[newIndex]->addParticle(newParticle, pool); // RECURSIVE CALL

        }

        else {
            // This node is already an internal node (has children)
            // Simply pass the new particle down to the appropriate child
            int targetIndex = box.getOctantIndex(newParticle->getPos());

            // Always subdivide to get the child box, even if children exist.
            // This ensures you pass the correct bounds if a child needs to be acquired.
            std::array<Box, 8> childBoxes = box.subdivide();

            if (children[targetIndex] == nullptr) {
                children[targetIndex] = pool.acquireNode(childBoxes[targetIndex]);
            }
            // RECURSIVE CALL
            children[targetIndex]->addParticle(newParticle, pool);
        }
    }


    // the primary calculate function
    void calculateForceOn(Particle* target_particle, double theta, double G) const {

        // first handle empty nodes
        if (isEmpty()) {
            return;
        }
        // handle self-interaction
        if (particle == target_particle && isLeaf()) {
            return;
        }

        // distance vector from particle of interest to current node
        Vec r_vec = target_particle->getPos() - getCenterOfMass();
        // squared
        double dist_sq = r_vec.magnitude() * r_vec.magnitude();

        // Particles at the same position
        // we will use a softening parameter EPSILON to prevent infinite forces

        // epsilon is our softening parameter
        if (dist_sq < std::numeric_limits<double>::epsilon()) {
            return;
        }
        double dist = r_vec.magnitude();

        // if (s/d < theta)
        if (approximationCondition(dist, theta) == true) {
            // if met, treat node as single point
            // calculate newton's law
            // F = G * m1 * m2 / r^2
            double force_magnitude = (G*target_particle->getMass()) * totalMass / dist_sq;

            // now calculate the direction vector
            Vec direction = r_vec.normalized();

            // force vector
            Vec force_vec = direction * force_magnitude;

            // add the force to the target particle's accumulated force
            target_particle->addAccumulatedForce(force_vec);
        }
        else {
            // only if it has children
            if (!isLeaf()) {
                for (Node* child: children) {
                    // calculate force w/ allocated children
                    if (child != nullptr) {
                        child->calculateForceOn(target_particle, theta, G);
                    }
                }
            }
            else {
                // must be a leaf at this point but s/d failed
                // too close to approximate
                if (particle != nullptr) {
                    Vec direct_r_vec = target_particle->getPos() - particle->getPos();
                    double direct_dist_sq = direct_r_vec.magnitude()*direct_r_vec.magnitude();
                    if (direct_dist_sq < std::numeric_limits<double>::epsilon()) {
                        return;
                        // avoids division by zero
                    }

                    double direct_force_magnitude = G * target_particle->getMass() * particle->getMass() / direct_dist_sq;
                    Vec direct_force_direction = direct_r_vec.normalized();
                    Vec direct_force_vec = direct_force_direction * direct_force_magnitude;
                    target_particle->addAccumulatedForce(direct_force_vec);

                }
            }
        }


    }

    // s/d < theta condition
    bool approximationCondition(double dist, double theta) const {
        double s = box.getSideLength();
        return (s/dist) < theta;
    }


};



#endif //NODE_H

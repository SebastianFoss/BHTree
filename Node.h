//
// Created by sailsec on 7/7/25.
//

#ifndef NODE_H
#define NODE_H

#include "Box.h"
#include "Vec.h"
#include <array>
#include <limits> // For std::numeric_limits
#include <cmath>  // For std::sqrt

#include "NodePool.h"
#include "Particle.h"

class NodePool;
struct Box;

class Node {

private:
    Box box;                  // The spatial bounding box of this node
    double totalMass;         // The total mass of all particles within this node's subtree
    Vec centerOfMass;         // The center of mass of all particles within this node's subtree
    Particle* particle;       // Points to a single particle if it's a leaf; nullptr otherwise
    std::array<Node*, 8> children; // NodePool-managed raw pointers to child nodes

    // Private helper: checks if ALL child pointers are null
    bool areAllChildrenNull() const {
        for (const Node* child : children) {
            if (child != nullptr) {
                return false;
            }
        }
        return true;
    }
    friend class NodePool; // Allows NodePool to access private members/constructor

    // Helper to update the node's total mass and center of mass incrementally
    void updateMassAndCenterOfMass(const Particle* p) {
        if (totalMass == 0.0) {
            centerOfMass = p->getPos();
            totalMass = p->getMass();
        } else {
            centerOfMass = ((centerOfMass * totalMass) + (p->getPos() * p->getMass())) / (totalMass + p->getMass());
            totalMass += p->getMass();
        }
    }

public:

    // Node constructor (now public for std::make_unique but still intended for NodePool use)
    Node(const Box& box_val)
        : box(box_val),
          totalMass(0.0),
          centerOfMass(Vec()),
          particle(nullptr) {
        for (Node*& child : children) {
            child = nullptr;
        }
    }

    // --- State Checkers ---
    bool isLeaf() const {
        return particle != nullptr || areAllChildrenNull();
    }
    bool isEmpty() const {
        return particle == nullptr && areAllChildrenNull();
    }
    bool isInternal() const { // Convenience helper
        return !isLeaf() && !isEmpty();
    }

    // --- Getters ---
    const Box& getBox() const {
        return box;
    }
    double getTotalMass() const {
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

    // Core node ops

    // Resets the node's state for reuse from the NodePool.
    void reset(const Box& new_box) {
        this->box = new_box;
        totalMass = 0.0;
        particle = nullptr;
        centerOfMass = Vec();
        for (Node*& child : children) {
            child = nullptr;
        }
    }

    void addParticle(Particle *newParticle, NodePool &pool);

    // Recursively calculates the force exerted by this node (or its subtree) on a target particle.
    void calculateForceOn(Particle* target_particle, double theta, double G) const {
        if (isEmpty()) {
            return;
        }
        if (particle == target_particle && isLeaf()) {
            return;
        }

        Vec r_vec = target_particle->getPos() - getCenterOfMass();
        double dist_sq = r_vec.magnitude_sq();

        if (dist_sq < std::numeric_limits<double>::epsilon()) {
            return;
        }
        double dist = std::sqrt(dist_sq);

        if (approximationCondition(dist, theta)) {
            double force_magnitude = (G * target_particle->getMass() * totalMass) / dist_sq;
            Vec direction = r_vec.normalized();
            Vec force_vec = direction * force_magnitude;
            target_particle->addAccumulatedForce(force_vec);
        } else {
            if (isInternal()) {
                for (Node* child : children) {
                    if (child != nullptr) {
                        child->calculateForceOn(target_particle, theta, G);
                    }
                }
            } else {
                if (particle != nullptr) {
                    Vec direct_r_vec = target_particle->getPos() - particle->getPos();
                    double direct_dist_sq = direct_r_vec.magnitude_sq();
                    if (direct_dist_sq < std::numeric_limits<double>::epsilon()) {
                        return;
                    }

                    double direct_force_magnitude = (G * target_particle->getMass() * particle->getMass()) / direct_dist_sq;
                    Vec direct_force_direction = direct_r_vec.normalized();
                    Vec direct_force_vec = direct_force_direction * direct_force_magnitude;
                    target_particle->addAccumulatedForce(direct_force_vec);
                }
            }
        }
    }

    // Helper function for the Barnes-Hut approximation criterion
    bool approximationCondition(double dist, double theta) const {
        double s = box.getSideLength();
        return (s / dist) < theta;
    }

    // Potential

    void calculatePotentialOn(Particle* target_particle, double theta, double G, double softening_epsilon) const {
        if (isEmpty()) {
            return;
        }
        if (particle == target_particle && isLeaf()) {
            return;
        }

        // calculate distance vec, and squared distance from target particle to this nodes CM
        Vec r_vec = target_particle->getPos() - getCenterOfMass();
        double dist_sq = r_vec.magnitude_sq();

        // apply the softening parameter and check for epsilon
        double softened_dist_sq = dist_sq + (softening_epsilon * softening_epsilon);
        if (softened_dist_sq < std::numeric_limits<double>::epsilon()) {
            return;
        }
        double dist = std::sqrt(dist_sq);
        // used for theta criterion

        // Barnes-Hut calculation
        if (approximationCondition(dist, theta)) {
            // if met,
            // potential = -G * M /
        }


    }


    void calculateAccelerationOn(Particle* target_particle, double theta, double G) const {

    }
};

#endif //NODE_H
//
// Created by sailsec on 7/7/25.
//

#ifndef BHTREE_H
#define BHTREE_H

#include <limits>
#include <vector>
#include <memory>

#include "Node.h"
#include "NodePool.h"
#include "Particle.h"


class BHtree {

private:

    const double G = 6.67430e-11; // Gravitational constant (adjust as needed)

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
    Box tree_bounds;
    // box has width, and corners represented by vecs




    // Children indeces vectorized -> associates particles with a location, and then associates them to a child node
    // have to check the coordinates to the quarter widths to verify their locations
    // quad: 0123, top left to br
    // Particles each have coordinates

    // pre: all particles must be valid
    // post: BHtree box (bounds) set to min and max coords
    void calculateTreeBounds() {
        if (particles.empty()) {
            throw std::invalid_argument("BHtree::calculateBounds: particles is empty");
        }

        double min_x = std::numeric_limits<double>::max();
        double min_y = std::numeric_limits<double>::max();
        double min_z = std::numeric_limits<double>::max();
        double max_x = std::numeric_limits<double>::lowest();
        double max_y = std::numeric_limits<double>::lowest();
        double max_z = std::numeric_limits<double>::lowest();

        for (const auto& p_ptr : particles) {
            const Vec& pos = p_ptr->getPos();
            min_x = std::min(min_x, pos.x);
            min_y = std::min(min_y, pos.y);
            min_z = std::min(min_z, pos.z);
            max_x = std::max(max_x, pos.x);
            max_y = std::max(max_y, pos.y);
            max_z = std::max(max_z, pos.z);
        }

        // Expand to a cubic bounding box, centered
        double dx = max_x - min_x;
        double dy = max_y - min_y;
        double dz = max_z - min_z;
        double max_dim = std::max(dx, dy, dz);

        // Add a small epsilon to ensure particles are strictly inside
        double padding = max_dim * 0.01;
        max_dim += padding;

        Vec center = Vec((min_x + max_x) / 2.0, (min_y + max_y) / 2.0, (min_z + max_z) / 2.0);
        tree_bounds = Box(center - Vec(max_dim / 2.0, max_dim / 2.0, max_dim / 2.0),
                          center + Vec(max_dim / 2.0, max_dim / 2.0, max_dim / 2.0));
    }

public:

    // Primary constructor
    // initializes node_pool
    // runs particles.reserve
    BHtree(std::vector<Particle> initial_particles) // Parameter by value
    // Initialize members in the correct order and with correct syntax
    : nodePool(std::make_unique<NodePool>(initial_particles.size() * 2))
    {
        // Sets the pointer array to the length of the particles array
        // Move the particles from the input vector directly into BHtree's unique_ptr vector
        particles.reserve(initial_particles.size()); // Pre-allocate memory
        for (auto& particle : initial_particles) { // Iterate over the vector passed by value
            particles.push_back(std::make_unique<Particle>(std::move(particle))); // Move each Particle
        }

        // Calculates the external-most bounds for the bounding box
        // This must be called *after* particles are populated.
        calculateTreeBounds();

        // The root node will be acquired and built in buildTree()
        // It's not part of the constructor's core responsibility to build the tree,
        // only to prepare for it.
    }

    void buildTree() {
        // 1. Release all nodes from the previous tree (if any) back to the pool
        nodePool->resetPool(); // This should make all nodes available for reuse
        root.reset(); // Release ownership of the previous root node

        // 2. Acquire a new root node from the pool, using the calculated tree bounds
        root = std::unique_ptr<Node>(nodePool->acquireNode(tree_bounds));
        if (!root) {
            throw std::runtime_error("Failed to acquire root node from NodePool.");
        }

        // 3. Insert all particles into the tree (this recursively builds the tree)
        for (const auto& p_ptr : particles) {
            // IMPORTANT: The Node::insertParticle method should handle the recursive subdivision
            // and placement of particles.
            root->addParticle(p_ptr.get(), *nodePool); // Renamed `addParticle` from `insertParticle` in Node
        }
    }

    // recursive calculation of force for all particles in the tree
    void calculateForces(double theta) {
        // 1. Reset accumulated forces/accelerations for all particles
        for (auto& p_ptr : particles) {
            p_ptr->resetAccumulatedForce();
        }

        // 2. For each particle, traverse the tree to calculate its total force
        for (auto& p_ptr : particles) {
            if (root) {
                root->calculateForceOn(p_ptr.get(), theta, G); // Pass G for force calculation
            }
        }
    }

    void step(double dt, double theta) {
        // 1. Build the tree for the current particle distribution
        buildTree();

        // 2. Calculate forces on all particles using the built tree
        calculateForces(theta);

        // 3. Update particle positions and velocities based on calculated forces
        for (auto& p_ptr : particles) {
            p_ptr->update(dt); // Particle::update should take dt and update pos/vel based on acc_force
        }
    }

};



#endif //BHTREE_H

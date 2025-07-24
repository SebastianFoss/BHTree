//
// Created by sailsec on 7/7/25.
//

#include "Node.h"

// Adds a particle to this node or recursively to one of its children.
    void Node::addParticle(Particle* newParticle, NodePool& pool) {
        updateMassAndCenterOfMass(newParticle);

        if (isEmpty()) {
            particle = newParticle;
        } else if (particle != nullptr) {
            Particle* oldParticle = particle;
            particle = nullptr;

            std::array<Box, 8> childBoxes = box.subdivide();

            int oldIndex = box.getOctantIndex(oldParticle->getPos());
            if (children[oldIndex] == nullptr) {
                children[oldIndex] = pool.acquireNode(childBoxes[oldIndex]);
            }
            children[oldIndex]->addParticle(oldParticle, pool);

            int newIndex = box.getOctantIndex(newParticle->getPos());
            if (children[newIndex] == nullptr) {
                children[newIndex] = pool.acquireNode(childBoxes[newIndex]);
            }
            children[newIndex]->addParticle(newParticle, pool);

        } else {
            int targetIndex = box.getOctantIndex(newParticle->getPos());
            std::array<Box, 8> childBoxes = box.subdivide();

            if (children[targetIndex] == nullptr) {
                children[targetIndex] = pool.acquireNode(childBoxes[targetIndex]);
            }
            children[targetIndex]->addParticle(newParticle, pool);
        }
    }
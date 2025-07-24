//
// Created by sailsec on 7/7/25.
//

#include "NodePool.h"

// Acquires a node from the pool. Resets its state with the given box.
    Node* NodePool::acquireNode(const Box& box) {
        if (free_nodes_pointers.empty()) {
            expandPoolMemory(); // Expand if no free nodes
            if (free_nodes_pointers.empty()) { // Check again after expansion
                throw std::runtime_error("NodePool failed to acquire node: No free nodes after expansion.");
            }
        }

        Node* node = free_nodes_pointers.back();
        free_nodes_pointers.pop_back();
        node->reset(box); // Reset the node with the new bounding box
        return node;
    }
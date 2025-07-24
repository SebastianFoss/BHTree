//
// Created by sailsec on 7/7/25.
//

#ifndef NODEPOOL_H
#define NODEPOOL_H

#include <memory> // For std::unique_ptr
#include <vector>
#include <stdexcept> // For std::runtime_error

#include "Box.h"
#include "Node.h"

class Node;

class NodePool {

private:
    // Member declarations are now *inside* the class scope
    std::vector<std::unique_ptr<Node>> all_nodes_storage; // The total storage of Node objects (owned)
    std::vector<Node*> free_nodes_pointers;              // List of raw pointers to currently free nodes

    // Expands the pool's memory when more nodes are needed.
    void expandPoolMemory() {
        size_t old_capacity = all_nodes_storage.size();
        size_t new_capacity = (old_capacity == 0) ? 100 : old_capacity * 2; // Double capacity, or start with 100

        all_nodes_storage.reserve(new_capacity); // Reserve new capacity to avoid reallocations

        // Add new nodes to the storage and to the free list
        for (size_t i = old_capacity; i < new_capacity; ++i) {
            all_nodes_storage.push_back(std::make_unique<Node>(Box())); // Node constructor is now public
            free_nodes_pointers.push_back(all_nodes_storage.back().get());
        }
    }

public:
    // Constructor: Pre-allocates initialCapacity nodes.
    NodePool(size_t initialCapacity) {
        all_nodes_storage.reserve(initialCapacity);
        for (size_t i = 0; i < initialCapacity; ++i) {
            all_nodes_storage.push_back(std::make_unique<Node>(Box()));
            free_nodes_pointers.push_back(all_nodes_storage.back().get());
        }
    }

    Node *acquireNode(const Box &box);

    // Releases a node back to the pool (adds it to the free list).
    void releaseNode(Node* node) { // Renamed from 'release' for clarity
        if (node != nullptr) {
            free_nodes_pointers.push_back(node);
        }
    }

    // Resets the pool, making all nodes available for reuse in the next tree build.
    void resetPool() {
        free_nodes_pointers.clear(); // Clear the current list of free nodes
        for (const auto& node_ptr : all_nodes_storage) {
            free_nodes_pointers.push_back(node_ptr.get()); // Add ALL nodes back to the free list
        }
    }

    // Destructor (unique_ptr handles memory, so default is fine).
    ~NodePool() = default;
};

#endif //NODEPOOL_H
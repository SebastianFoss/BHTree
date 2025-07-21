//
// Created by sailsec on 7/7/25.
//

#ifndef NODEPOOL_H
#define NODEPOOL_H
#include <memory>
#include <vector>

#include "Node.h"


class NodePool {

private:
    std::vector<std::unique_ptr<Node>> pool_memory; // the total storage of nodes
    std::vector<Node*> free_nodes; // list of currently free nodes
    size_t nextAvailableID;


public:

    // modifies pool_memory
    // reserve(initialCapacity) -- reserves heap allocation in mem
    // emplace_back() -- calls the Node constructor for each node within pool_memory
    // free_nodes.push_back(...) adds pointers to Nodes to the free list, frees memory.
    NodePool(size_t initialCapacity) : nextAvailableID(0) {
        pool_memory.reserve(initialCapacity);
        for (size_t i = 0; i < initialCapacity; i++) {
            // default box nodes
            pool_memory.push_back(std::make_unique<Node>(Box()));
            // add raw pointer to free nodes list
            free_nodes.push_back(pool_memory.back().get());
        }
    }


    // pops a node from free_nodes for use, and calls node->reset
    Node* acquireNode(const Box& box) {
        if (!pool_memory.empty()) {
            Node* node = free_nodes.back();
            free_nodes.pop_back();
            node->reset(box);
            return node;
        }
        else {
            expandMemory();
            acquireNode(box);
        }
    }


    // expands memory by 2x
    void expandMemory() {
        size_t size = pool_memory.size();
        pool_memory.reserve(size);
        for (size_t i = 0; i < size; i++) {
            pool_memory.push_back(std::make_unique<Node>(Box()));
            free_nodes.push_back(pool_memory.back().get());
        }
    }



    // pushes a node back onto free_nodes.
    void release(Node* node) {
        // error checking needed
        if (node != nullptr) {
            free_nodes.push_back(node);
        }
    }


    // unsure?
    // runs release for all nodes.
    void resetPool() {
        // 1. Clear the current list of free nodes.
        // This is important because free_nodes_pointers might only contain
        // a subset of all_nodes_storage if some nodes are currently in use.
        free_nodes.clear();

        // 2. Add raw pointers of ALL owned Node objects back to the free list.
        // This effectively marks all nodes as "available" for the next tree build.
        for (const auto& node_ptr : pool_memory) {
            free_nodes.push_back(node_ptr.get());
        }
    }





    // destroy nodepool, which calls pool_memory destructor.
    ~NodePool();

};



#endif //NODEPOOL_H

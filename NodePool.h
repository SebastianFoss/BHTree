//
// Created by sailsec on 7/7/25.
//

#ifndef NODEPOOL_H
#define NODEPOOL_H
#include <vector>

#include "Node.h"


class NodePool {

private:
    std::vector<Node> pool_memory; // the total storage of nodes
    std::vector<Node*> free_nodes; // list of currently free nodes


public:

    // modifies pool_memory
    // reserve(initialCapacity) -- reserves heap allocation in mem
    // emplace_back() -- calls the Node constructor for each node within pool_memory
    // free_nodes.push_back(...) adds pointers to Nodes to the free list, frees memory.
    NodePool(size_t initSize);


    // pops a node from free_nodes for use, and calls node->reset
    Node* acquire();



    // pushes a node back onto free_nodes.
    void release(Node* node);

    // runs release for all nodes.
    void releaseAllNodes();





    // destroy nodepool, which calls pool_memory destructor.
    ~NodePool();

};



#endif //NODEPOOL_H

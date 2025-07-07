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

};



#endif //NODEPOOL_H

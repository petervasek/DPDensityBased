#ifndef DP_EDGE_H
#define DP_EDGE_H

#include <memory>

class Cell;

class Edge {
public:
    std::shared_ptr<Cell> firstCell, secondCell;
};


#endif //DP_EDGE_H

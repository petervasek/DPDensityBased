#ifndef DP_CELL_H
#define DP_CELL_H

#include <vector>
#include <memory>
#include <string.h>
#include "../main.h"
#include "DataSet.h"

class Point;
class Level;

class Cell {
public:
    std::vector<std::shared_ptr<Point>> points;
    long pointCounter;
    double cellGridSize;
    double *coord;
    int *coordIndex;
    bool coreCell;
    int cellId;                 //union find
    int cellParent;             //union find
    std::string cellParentKey;
    int clusterIndex;
    std::string cellKey;
    bool modified;

public:
    Cell();
    ~Cell();
    bool checkIfIsSameCluster(std::shared_ptr<Cell> targetCell, int dim);
    void addCellToCluster(long clusterIndex);
    bool isCoreCell();
    std::string getKey();
};

#endif //DP_CELL_H

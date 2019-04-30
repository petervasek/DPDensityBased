#ifndef DP_CELL_H
#define DP_CELL_H

#include <vector>
#include <memory>
#include <string.h>
#include "Edge.h"
#include "../main.h"
#include "DataSet.h"

class Point;
class Edge;
class Level;

class Cell {
public:
    std::vector<std::shared_ptr<Edge>> edges;       //for version without GDPAM
    std::vector<std::shared_ptr<Point>> points;
    long pointCounter;
    double cellGridSize;
    double coord[DIMENSION];
    int coordIndex[DIMENSION];
    bool coreCell;
    int cellId;                 //union find
    int cellParent;             //union find
    std::string cellParentKey;
    int clusterIndex;
    std::string cellKey;

public:
    void addEdges(Level *specificLevel, double maxEps, DataSet input);
    void expandCell(Level *cellLevel);
    void recursiveExpandCell(Level *cellLevel, int actualDim, double *axis);
    void extendPoints(Level *lowerLevel);
    bool checkIfIsSameCluster(std::shared_ptr<Cell> targetCell);
    void addCellToCluster(long clusterIndex);
    bool isCoreCell();
    std::string getKey();
};

#endif //DP_CELL_H

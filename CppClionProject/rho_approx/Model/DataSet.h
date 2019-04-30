#ifndef DP_DATASET_H
#define DP_DATASET_H

#include <vector>
#include <memory>
#include "Point.h"

class Cluster;
class Cell;
class Level;
class Edge;

class DataSet {
public:
    double getPointDistance(Point *p1, Point *p2);
    double getCellDistance(Cell *firstCell, Cell *secondCell, double maxEps);
    void getInputPoints(std::vector<std::shared_ptr<Point>> &inputPoints, double *bordersMax, double *bordersMin);
    void printAllClusters(std::vector<Cluster*> clusters);
    void printNoise(std::vector<Point> &input);
    void fillClusters(Level *specificLevel, std::vector<Cluster> clusters);
    void connectClusters(Level *specificLevel);
    void unionCells(Edge *clusterConnect);
    void assignPoints(std::vector<std::shared_ptr<Point>> &inputPoints, Level &specificLevel);
    void getClusterOf(std::shared_ptr<Point> &p, std::vector<std::shared_ptr<Level>> allLevels, int *numOfClusters);
};

#endif //DP_DATASET_H

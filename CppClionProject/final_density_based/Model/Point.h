#ifndef DP_POINT_H
#define DP_POINT_H

#include <string>
#include <vector>
#include <memory>

class Cell;

class Point {
public:
    enum GroupStatus {inCluster, noise, undecided};
    enum ProcessStatus {visited, unvisited};

public:
    double *coord;
    bool corePoint;
    ProcessStatus st;
    GroupStatus gr;
    int clusterIndex;

public:
    Point(int dim);
    ~Point();
    std::string toString(int dim);
    double getDistance(Point *p, int dim);
    double getDistance(Cell *p, int dim);
    bool isCorePoint();
};

#endif //DP_POINT_H

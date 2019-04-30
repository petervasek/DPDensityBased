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
    Point();
    ~Point();
    void tagNeighbours(Point actual, std::vector<std::shared_ptr<Point>> &inputPoints, std::vector<std::shared_ptr<Point>> &neighbours, double maxEps);
    std::string toString();
    double getDistance(Point *p);
    double getDistance(Cell *p);
    std::string getCoord();
    bool isCorePoint();
};

#endif //DP_POINT_H

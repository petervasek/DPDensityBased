#ifndef DP_CLUSTER_H
#define DP_CLUSTER_H

#include <vector>
#include <string>
#include "Cell.h"

class Cluster {
public:
    std::vector<std::shared_ptr<Cell>> cells;
    int id;
    long numOfPoints;


    Cluster expand(std::vector<Point*> &neighbours, Cluster &actualCluster, double maxEps, int minPoints, std::vector<Point> &input);
    std::string printCluster(Cluster &actualCluster);
};


#endif //DP_CLUSTER_H

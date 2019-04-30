#ifndef DP_CLUSTER_H
#define DP_CLUSTER_H

#include <vector>
#include <string>
#include "Cell.h"

#include <Rcpp.h>
using namespace Rcpp;

class Cluster {
public:
  std::vector<std::shared_ptr<Cell>> cells;
  int id;
  long numOfPoints;

  std::string printCluster(int dim);
};



#endif //DP_CLUSTER_H

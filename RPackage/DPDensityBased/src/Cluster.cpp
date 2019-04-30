#include <iostream>
#include "Cluster.h"
#include "Point.h"

#include <Rcpp.h>
using namespace Rcpp;

std::string Cluster::printCluster(int dim) {
  for (auto &cell : this->cells) {
    for (int j = 0; j < cell->points.size(); j++) {
      for (int k = 0; k < dim; k++)
        std::cout << cell->points[j]->coord[k] << " ";

      if (cell->points[j]->st == 0)
        std::cout << "visited, ";
      else
        std::cout << "unvisited, ";

      if (cell->points[j]->gr == 0)
        std::cout << "inCluster";
      else if (cell->points[j]->gr == 1)
        std::cout << "noise";
      else
        std::cout << "undecided";
      std::cout << "\n";
    }
  }
  getchar();
};

#include <iostream>
#include <cmath>
#include <limits>
#include "Cell.h"
#include "Level.h"
#include "DataSet.h"

#include <Rcpp.h>
using namespace Rcpp;

Cell::Cell(){
  modified = false;
};

Cell::~Cell(){
  delete[] coord;
  delete[] coordIndex;
};

bool Cell::checkIfIsSameCluster(std::shared_ptr<Cell> targetCell, int dim) {

  for (auto &point : this->points) {
    for (auto &j : targetCell->points) {
      if(point.get()->getDistance(j.get(), dim) < EPS){
        return true;
      }
    }
  }

  return false;
}

void Cell::addCellToCluster(long newIndex) {

  for (auto &point : this->points) {
    if (point->clusterIndex == -1) {
      point->clusterIndex = newIndex;
    }
  }
}

bool Cell::isCoreCell() {
  return coreCell;
}

std::string Cell::getKey(){
  return cellKey;
}

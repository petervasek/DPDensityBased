#include <cmath>
#include <sstream>
#include "Point.h"
#include "main.h"
#include "Cell.h"

#include <Rcpp.h>
using namespace Rcpp;
// [[Rcpp::plugins(openmp)]]

Point::Point(int dim){
  coord = new double[dim];
};


Point::~Point(){
  delete[] coord;
};

std::string Point::toString(int dim) {
  std::string result;
  std::ostringstream s;

  for (int i = 0; i < dim; i++) {
    s << coord[i];
    s << " ";
  }
  result += s.str();

  return result;
}

double Point::getDistance(Point *p, int dim) {

  double result = 0;

#ifdef OPENMP
#pragma omp parallel shared(dim, result)
{
#pragma omp for schedule(dynamic)
#endif
  //Euclidean distance
  for (int i = 0; i < dim; i++) {
#ifdef OPENMP
#pragma omp critical
#endif
    result += pow(coord[i] - p->coord[i], 2);
  }
#ifdef OPENMP
}
#endif

return sqrt(result);
}

double Point::getDistance(Cell *c, int dim) {
  double result = 0;

  //Euclidean distance
  for(int i = 0 ; i<dim ; i++) {
    result += pow(coord[i] - c->coord[i], 2);
  }

  return sqrt(result);
}

bool Point::isCorePoint() {
  return corePoint;
}

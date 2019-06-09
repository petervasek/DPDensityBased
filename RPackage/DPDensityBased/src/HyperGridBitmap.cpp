#include <iostream>
#include <cmath>
#include "HyperGridBitmap.h"

#include <Rcpp.h>
using namespace Rcpp;
// [[Rcpp::plugins(openmp)]]

HyperGridBitmap::HyperGridBitmap() {
  rowSize = 0;
  columnSize = 0;
}

HyperGridBitmap::~HyperGridBitmap() {
  for (int i = 0; i < rowSize; i++) {
    delete[] coreGrids[i];
  }
  delete[] coreGrids;
}

void HyperGridBitmap::initTable(int dimIndex, Level *cellLevel, double *bordersMax, double *bordersMin) {

  double gridSize = cellLevel->getLevelGridSize();

  double sizeInDim = bordersMax[dimIndex] - bordersMin[dimIndex];
  if(gridSize <= 0 || sizeInDim < 0){
    return;
  }

  rowSize = (int) (ceil(sizeInDim / gridSize));
  columnSize = (cellLevel->getNumOfCells());

  coreGrids = new bool *[rowSize];
  for (int i = 0; i < rowSize; ++i) {
    coreGrids[i] = new bool[columnSize]();
  }
}

void HyperGridBitmap::setCellPosition(int dimIndex, Level *cellLevel, int maxIndex) {

  //number of columns is equal to number of cells, it does not matter which array we iterate through
  int i = 0;
  for (auto &cell : cellLevel->getCells()) {
    if (cell.second.get()->coordIndex[dimIndex] > maxIndex || cell.second.get()->coordIndex[dimIndex] < 0) {
      throw std::out_of_range ("cell out of bounds");
    }
    //row with right index for current cell gets true
    coreGrids[cell.second.get()->coordIndex[dimIndex]][i] = true;
    i++;
  }
}

void HyperGridBitmap::getNeighbours(int dimIndex, std::shared_ptr<Cell> target, int *dimResultVector, int numOfCells, int dim) {

  int dimSqrtCeil = (int) floor(sqrt(dim));
  int lowerBound, upperBound;

  lowerBound = target.get()->coordIndex[dimIndex] - dimSqrtCeil;
  if(lowerBound < 0) {
    lowerBound = 0;
  }

  upperBound = target.get()->coordIndex[dimIndex] + dimSqrtCeil;
  if(upperBound > rowSize-1) {
    upperBound = rowSize-1;
  }


  for (int i = lowerBound; i <= upperBound; i++) {
    for(int j = 0 ; j<numOfCells ; j++) {
      if (coreGrids[i][j]) {
#ifdef OPENMP
#pragma omp critical
#endif
        dimResultVector[j]++;
      }
    }
  }
}

int HyperGridBitmap::getRowSize() {
  return rowSize;
}

int HyperGridBitmap::getColumnSize() {
  return columnSize;
}

int HyperGridBitmap::getCoreGrid(int row, int column) {
  return coreGrids[row][column];
}



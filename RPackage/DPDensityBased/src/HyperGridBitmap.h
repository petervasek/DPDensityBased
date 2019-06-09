#include "Level.h"

#include <Rcpp.h>
using namespace Rcpp;

class HyperGridBitmap {
public:
  //x axis -columnSize- non empty cells
  //y axis -rowSize- all positions with cell start coordinates
  int rowSize, columnSize;
  bool** coreGrids;

public:
  HyperGridBitmap();
  ~HyperGridBitmap();
  void initTable(int dimIndex, Level *cellLevel, double *bordersMax, double *bordersMin);
  void setCellPosition(int dimIndex, Level *cellLevel, int maxIndex);
  void getNeighbours(int dimIndex, std::shared_ptr<Cell> target, int *dimResultVector, int numOfCells, int dim);
  int getRowSize();
  int getColumnSize();
  int getCoreGrid(int row, int column);
};

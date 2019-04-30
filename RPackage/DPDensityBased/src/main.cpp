#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include <memory>
#include <wtypesbase.h>
#include <omp.h>
#include <ctime>
#include "Cluster.h"
#include "Point.h"
#include "Level.h"
#include "HyperGridBitmap.h"
#include "main.h"

#include <Rcpp.h>
using namespace Rcpp;
// [[Rcpp::plugins(openmp)]]

typedef std::map<int, Cluster*> ClusteringResult;

// [[Rcpp::export]]
int main() {

    omp_set_num_threads(4);


  //algorithm parameters
  int minPts = MIN_PTS;
  double maxEps = EPS;
  std::string input_file = INPUT_FILE;

  clock_t start = clock();
  DataSet input;
  DataSet deleteInput;
  Level cellLevel;

  int numOfDim;
  double gridSize;
  long cellIdCounter = 0;


#ifdef LOAD_CLUSTERING_RESULT
  cellLevel.loadResultFromFile(LOAD_PATH, &input);
  numOfDim = cellLevel.getNumOfDimensions();
#endif

  //INIT DATA STRUCTURES

#ifdef LOAD_NEW_DATASET
#ifdef DELETE_INPUTTED
  deleteInput.loadInputPoints(input_file);
#else
  input.loadInputPoints(input_file);

  numOfDim = input.getNumOfDimensions();
  gridSize = maxEps / sqrt(numOfDim);
  cellLevel.setLevelGridSize(gridSize);
  cellLevel.setNumOfDimensions(numOfDim);
  cellLevel.initFirstLevel(&input);
#endif


#endif

  //GDPAM
  HyperGridBitmap gdpamTables[numOfDim];

#ifdef OPENMP
#pragma omp parallel shared(numOfDim, cellLevel, input)
{
#pragma omp for schedule(dynamic)
#endif
  //init table for each dimension
  for (int i = 0; i < numOfDim; i++) {
    gdpamTables[i].initTable(i, &cellLevel, input.getCoordBordersMax(), input.getCoordBordersMin());
  }
#ifdef OPENMP
}
#endif
std::cout << "GDPAM tables successfully initiated\n";


#ifdef OPENMP
#pragma omp parallel shared(numOfDim, cellLevel)
{
#pragma omp for schedule(dynamic)
#endif
  for (int i = 0; i < numOfDim; i++) {
    gdpamTables[i].setCellPosition(i, &cellLevel);
  }
#ifdef OPENMP
}
#endif
std::cout << "GDPAM cell positions set\n";


int maxOldId = 0;

#ifdef LOAD_CLUSTERING_RESULT
for(auto &cell : cellLevel.cells) {
  if(cell.second.get()->cellId > maxOldId) {
    maxOldId = cell.second.get()->cellId + 1;
  }
}
std::cout << "Max old id: " << maxOldId << "\n";
#endif

#ifndef DELETE_INPUTTED
int i = maxOldId+1;
for(auto &cell : cellLevel.cells) {
  if(cell.second.get()->cellId == -1) {
    cell.second.get()->cellId = i;
    i++;
  }
}
#endif
std::cout << "Gathering results cellId set\n";

int coreCellIndex = 0;
int outputCounter = 0;
int *dimResultVector = (int*) calloc(static_cast<size_t>(cellLevel.getNumOfCells()), sizeof(int));

#ifdef DELETE_INPUTTED

for(int i = 0 ; i<deleteInput.getInputPoints().size() ; i++) {
  auto point = deleteInput.getInputPoints().at(i);
  auto cell = cellLevel.findCell(point, input.getCoordBordersMin(), numOfDim);

  std::vector<std::shared_ptr<Cell>> neighbours;

  if (cell != nullptr) {
    for (int j = 0; j < cell.get()->pointCounter; j++) {
      int simCounter = 0;
      for (int k = 0; k < numOfDim; k++) {
        if (std::abs(cell.get()->points.at(j).get()->coord[k] - point.get()->coord[k]) < 0.01) {
          simCounter++;
        }
      }

      //found point to delete
      if (simCounter == numOfDim) {

        for (int k = 0; k < numOfDim; k++) {
          gdpamTables[k].getNeighbours(k, cell, dimResultVector, cellLevel.getNumOfCells(),
                                       cellLevel.getNumOfDimensions());
        }

        int neighbourPoints = cell.get()->pointCounter;
        int l = 0;

        for (auto &c : cellLevel.cells) {
          if (dimResultVector[l] == numOfDim) {
            neighbourPoints += c.second->pointCounter;
            neighbours.push_back(c.second);
          }
          l++;
        }

        //still core cell, change wont affect other cells
        if (cell.get()->pointCounter > minPts) {
          input.getInputPoints().erase(std::remove(input.getInputPoints().begin(), input.getInputPoints().end(),
                                       cell.get()->points.at(j)), input.getInputPoints().end());
          cell.get()->pointCounter--;
          cell.get()->points.erase(cell.get()->points.begin() + j);
        }
        //already noise, change wont affect other cells
        else if(cell.get()->cellParent == -1){
          input.getInputPoints().erase(std::remove(input.getInputPoints().begin(), input.getInputPoints().end(),
                                       cell.get()->points.at(j)), input.getInputPoints().end());
          cell.get()->pointCounter--;
          cell.get()->points.erase(cell.get()->points.begin() + j);
        }
        else if (neighbourPoints > minPts) {
          input.getInputPoints().erase(std::remove(input.getInputPoints().begin(), input.getInputPoints().end(),
                                       cell.get()->points.at(j)), input.getInputPoints().end());
          cell.get()->pointCounter--;
          cell.get()->points.erase(cell.get()->points.begin() + j);

          /*for (auto &n : neighbours) {
          n.get()->modified = true;
        }*/
      }
        else if (neighbourPoints <= minPts) {
          input.getInputPoints().erase(std::remove(input.getInputPoints().begin(), input.getInputPoints().end(),
                                       cell.get()->points.at(j)), input.getInputPoints().end());
          cell.get()->pointCounter--;
          cell.get()->points.erase(cell.get()->points.begin() + j);

          for (auto &n : neighbours) {
            n.get()->modified = true;
            n.get()->cellParent = n.get()->cellId;
          }
        }

      }
    }
    dimResultVector = (int *) memset(dimResultVector, 0, cellLevel.getNumOfCells() * sizeof(int));
  }
  neighbours.clear();
}

for(auto &cell : cellLevel.cells) {
  if (cell.second.get()->pointCounter == 0) {
    std::vector<std::shared_ptr<Cell>> neighbours;

    for (int k = 0; k < numOfDim; k++) {
      gdpamTables[k].getNeighbours(k, cell.second, dimResultVector, cellLevel.getNumOfCells(),
                                   cellLevel.getNumOfDimensions());
    }

    int neighbourPoints = cell.second.get()->pointCounter;
    int l = 0;

    for (auto &c : cellLevel.cells) {
      if (dimResultVector[l] == numOfDim) {
        neighbourPoints += c.second->pointCounter;
        neighbours.push_back(c.second);
      }
      l++;
    }
    for (auto &n : neighbours) {
      n.get()->modified = true;
      if (n.get()->cellParent != -1) {
        n.get()->cellParent = n.get()->cellId;
      }
    }
  }
}

for(auto &cell : cellLevel.cells) {
  if (cell.second.get()->pointCounter == 0) {
    cellLevel.cells.erase(cell.second.get()->cellKey);
  }
}

//reinit gdpam
for (int i = 0; i < numOfDim; i++) {
  gdpamTables[i].initTable(i, &cellLevel, input.getCoordBordersMax(), input.getCoordBordersMin());
}
for (int i = 0; i < numOfDim; i++) {
  gdpamTables[i].setCellPosition(i, &cellLevel);
}

#endif

dimResultVector = (int *) memset(dimResultVector, 0, cellLevel.getNumOfCells() * sizeof(int));

for(auto &cell : cellLevel.cells) {
  outputCounter++;
  if(outputCounter % 1000 == 0){
    std::cout << "-- Processed: " << outputCounter << " / " << cellLevel.cells.size() << "\n";
  }

  //check only unassigned cells
  if (cell.second.get()->cellParent != -1 and !cell.second.get()->modified) {
    continue;
  }


#ifdef OPENMP
#pragma omp parallel shared(numOfDim, cell, dimResultVector, cellLevel)
{
#pragma omp for schedule(dynamic)
#endif
  for (int j = 0; j < numOfDim; j++) {
    gdpamTables[j].getNeighbours(j, cell.second, dimResultVector, cellLevel.getNumOfCells(),
                                 cellLevel.getNumOfDimensions());
  }
#ifdef OPENMP
}
#endif

std::vector<std::shared_ptr<Cell>> neighbours;
int neighbourPoints = cell.second.get()->pointCounter;
int j = 0;

for (auto &c : cellLevel.cells) {
  if (dimResultVector[j] == numOfDim) {
    neighbourPoints += c.second->pointCounter;
    neighbours.push_back(c.second);
  }
  j++;
}

//cluster wont be created - not enough points in Eps distance of cell
if (neighbourPoints < minPts) {
  neighbours.clear();
  cell.second.get()->cellParent = -1;
  dimResultVector = (int *) memset(dimResultVector, 0, cellLevel.getNumOfCells()*sizeof(int));
  continue;
}

Cell *currentCell = cell.second.get();
currentCell->cellParent = currentCell->cellId;

#ifdef OPENMP
#pragma omp parallel shared(neighbours, currentCell)
{
#pragma omp for schedule(dynamic)
#endif

  for (int k = 0 ; k<neighbours.size() ; k++) {
    if(neighbours.at(k).get()->cellParent == -1){
      neighbours.at(k).get()->cellParent = currentCell->cellId;
      neighbours.at(k).get()->cellParentKey = currentCell->cellKey;
    }
    else{
      Cell *parent = neighbours.at(k).get();
#ifdef OPENMP
#pragma omp critical
{
#endif
  while(parent->cellParent != parent->cellId){
    parent = cellLevel.cells.find(parent->cellParentKey)->second.get();
  }
  parent->cellParent = currentCell->cellId;
  parent->cellParentKey = currentCell->cellKey;
#ifdef OPENMP
}
#endif
    }
  }
#ifdef OPENMP
}
#endif
//std::cout <<  neighbours.size() <<"\n";
neighbours.clear();
//std::cout << coreCellIndex << ". core cell processed\n";
dimResultVector = (int *) memset(dimResultVector, 0, cellLevel.getNumOfCells()*sizeof(int));
coreCellIndex++;
}
free(dimResultVector);
std::cout << "Cluster indexes assigned \n";

int noise = 0;
for (auto &cell : cellLevel.cells) {
  Cell *c = cell.second.get();
  if(c->cellParent == -1){
    noise += c->pointCounter;
    continue;
  }
  while(c->cellParent != c->cellId) {
    c = cellLevel.cells.find(c->cellParentKey)->second.get();
  }
#ifdef OPENMP
#pragma omp parallel shared(c, cell)
{
#pragma omp for schedule(dynamic)
#endif
  for (unsigned int i = 0 ; i<cell.second.get()->points.size() ; i++) {
    cell.second.get()->points.at(i).get()->clusterIndex = c->cellParent;
  }
#ifdef OPENMP
}
#endif
}

std::cout << "\n--Noise size: " << noise << "\n";

//gather results
ClusteringResult clusters;
int idCounter = 1;
for(auto &c : cellLevel.getCells()) {
  if(c.second.get()->cellParent != -1) {
    Cell *cParent = c.second.get();
    while(cParent->cellParent != cParent->cellId) {
      cParent = cellLevel.cells.find(cParent->cellParentKey)->second.get();
    }
    if ( clusters.find(cParent->cellParent) == clusters.end() ) {
      clusters.emplace(cParent->cellParent, new Cluster);
      clusters[cParent->cellParent]->id = idCounter;
      idCounter++;

    }
    clusters[cParent->cellParent]->cells.emplace_back(c.second);
    clusters[cParent->cellParent]->numOfPoints += c.second.get()->pointCounter;
  }
}
i = 1;
for(auto &a : clusters) {
  std::cout << "cluster no. " << i << " cell size: " << a.second->cells.size() << "\n";
  i++;
}

std::ofstream file;
file.open(input.getOutputName("clustering_output", input_file, maxEps, minPts));
for (auto &inputPoint : input.getInputPoints()) {
  file << inputPoint->toString(numOfDim);
  file << inputPoint->clusterIndex << "\n";
}
file.close();


file.open(input.getOutputName("clustering_output_cells", input_file, maxEps, minPts));
for (auto &cl : clusters) {
  for (auto &cell : cl.second->cells) {
    for(int j = 0 ; j<numOfDim ; j++) {
      file << cell.get()->coord[j] << " ";
    }
    file << cl.second->id << "\n";
  }
}
file.close();


#ifdef SAVE_CLUSTERING_RESULT
for (auto &i : input.getInputPoints()) {
  i.get()->st = Point::visited;
}

cellLevel.saveResultToFile(input.getOutputName("clustering_state", input_file, maxEps, minPts), &input);
#endif

clock_t stop = clock();
double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
std::cout << "\nTime elapsed: " << elapsed;
return 0;
}

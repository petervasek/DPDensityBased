#include <iostream>
#include <cmath>
#include <omp.h>
#include <fstream>
#include <ctime>
#include "Level.h"
#include "Cell.h"

#include <Rcpp.h>
using namespace Rcpp;
// [[Rcpp::plugins(openmp)]]

Level::Level(){
  cells.clear();
}

Level::~Level(){
  cells.clear();
};

std::shared_ptr<Cell> Level::findCell(std::shared_ptr<Point> &point, double *bordersMin, int dim){

  std::string pointKey = "";
  int index;

  for (int j = 0; j < dim; j++) {
    index = (int) floor((point->coord[j] - bordersMin[j]) / levelGridSize);
    pointKey += std::to_string(index);
    pointKey += ",";
  }

  if ( cells.find(pointKey) == cells.end() ) {
    return nullptr;
  } else {
    return cells.find(pointKey)->second;
  }
}

void Level::initFirstLevel(DataSet *input){

  //pomocny vypis
  long cubeNum = 1;
  for(int i = 0 ; i < input->getNumOfDimensions() ; i++) {
    std::cout << "dim " << i+1 << " min:\t" << input->getCoordBordersMin()[i] << "\t\tmax:" << input->getCoordBordersMax()[i] << "\n";
    std::cout << cubeNum << " " << ceil(((input->getCoordBordersMax()[i] - input->getCoordBordersMin()[i]) / levelGridSize))  << "\n";
    if((input->getCoordBordersMax()[i] - input->getCoordBordersMin()[i]) / levelGridSize  > 1)
      cubeNum *= (int) ceil(((input->getCoordBordersMax()[i] - input->getCoordBordersMin()[i]) / levelGridSize));
  }
  std::cout << "gridSize:\t" << levelGridSize ;
  std::cout << "\npocet n-kociek:\t" << cubeNum << "\n\n";

  //trying to obtain n-dimensional cube (composed of first level cells)
  //iterating over whole space containing points
  if (levelGridSize <= 0) {
    throw std::out_of_range ("gridSize out of range");
  }
  unsigned int actualDim = 0;
  double actualCoord[input->getNumOfDimensions()];
  int actualCoordIndex[input->getNumOfDimensions()];
  int cellNums[input->getNumOfDimensions()];

#ifdef OPENMP
#pragma omp parallel shared(input, cellNums, actualCoord, actualCoordIndex)
{
#pragma omp for schedule(dynamic)
#endif
  for (unsigned int i = 0; i < input->getNumOfDimensions(); i++) {
    cellNums[i] = (int) ceil((input->getCoordBordersMax()[i] - input->getCoordBordersMin()[i]) / levelGridSize);
    std::cout << "\n" << i << ". dim = " << cellNums[i] << " ";
    actualCoord[i] = input->getCoordBordersMin()[i];     //starting coordinates in every dimension set to min value
    actualCoordIndex[i] = 0;
  }
  std::cout << "\n";
#ifdef OPENMP
}
#endif

long pointCounter = 0;

#ifdef OPENMP
#pragma omp parallel shared(input)
{
#pragma omp for schedule(dynamic)
#endif
  for (unsigned int i = 0; i < input->getInputPoints().size(); i++) {
    auto point = input->getInputPoints().at(i);
    if(point.get()->st == Point::visited){
      continue;
    }
    auto cell = findCell(point, input->getCoordBordersMin(), input->getNumOfDimensions());

    if (cell != nullptr) {
      cell->pointCounter++;
#ifdef OPENMP
#pragma omp critical
{
#endif
  cell->points.emplace_back(point);
#ifdef OPENMP
}
#endif
    }
    else {
      //vytvorit bunku na spravnom mieste
      auto initCell = std::make_shared<Cell>();
      initCell->coord = new double[input->getNumOfDimensions()];
      initCell->coordIndex = new int[input->getNumOfDimensions()];
      initCell->cellGridSize = levelGridSize;
      initCell->coreCell = false;
      initCell->pointCounter = 0;
      initCell->cellParent = -1;
      initCell->cellParentKey = "X";
      initCell->clusterIndex = -1;
      initCell->cellId = -1;
#ifdef LOAD_CLUSTERING_RESULT
      initCell->modified = true;
#endif
      for (int j = 0; j < input->getNumOfDimensions(); j++) {
        //spravne nastavit koordinaty a index zaciatku bunky
        for (int k = 0; k < cellNums[j]; k++) {
          if (point->coord[j] >= (levelGridSize * k + input->getCoordBordersMin()[j]) &&
              point->coord[j] < (levelGridSize * (k + 1) + input->getCoordBordersMin()[j])) {
            initCell->coord[j] = (levelGridSize * k + input->getCoordBordersMin()[j]);
            initCell->coordIndex[j] = k;
            break;
          }
        }
      }
      initCell->cellKey = "";
      for (int j = 0; j < input->getNumOfDimensions(); j++) {
        initCell->cellKey += std::to_string(initCell->coordIndex[j]);
        initCell->cellKey += ",";
      }

      initCell->pointCounter++;
      initCell->points.emplace_back(point);
#ifdef OPENMP
#pragma omp critical
{
#endif
  cell = findCell(point, input->getCoordBordersMin(), input->getNumOfDimensions());
  if (cell != nullptr) {
    cell->points.emplace_back(point);
  }
  else {
    cells[initCell->cellKey] = initCell;
  }
#ifdef OPENMP
}
#endif
    }
    pointCounter++;
  }
#ifdef OPENMP
}
#endif
std::cout << "\ninput size: " << input->getInputPoints().size() << "\tpoints assigned: " << pointCounter << "\n\n\n";

}

int Level::saveResultToFile(std::string fileName, DataSet *input) {

  std::ofstream file;
  file.open(fileName);
  std::string header = "//Clustering result - created ";
  time_t now = time(nullptr);
  char* dt = ctime(&now);
  header += dt;
  header += "//Number of points in dataset: ";
  header += std::to_string(input->getInputPoints().size());
  header += "\n\n";


  file << header;
  file << "data_dimensionality: " << this->getNumOfDimensions() << "\n";
  file << "level_grid_size: " << levelGridSize << "\n";

  //num of cells
  file << "level_cell_num: " << getNumOfCells() << "\n";

  //min_max coords
  for (int i = 0 ; i<this->getNumOfDimensions() ; i++) {
    file << input->getCoordBordersMin()[i] << " ";
  }
  file << "\n";
  for (int i = 0 ; i<this->getNumOfDimensions() ; i++) {
    file << input->getCoordBordersMax()[i] << " ";
  }
  file << "\n";

  //statistics for each cell, then list of its point
  for (auto &cell : cells) {
    file << "CELL_START\n";
    file << cell.first << "\n";                             //"cell_map_id: " <<
    file << cell.second.get()->pointCounter << "\n";        // "cell_pointCounter: " <<
    file << cell.second.get()->cellGridSize << "\n";        // "cell_cellGridSize: " <<
    //file << "cell_coord: ";
    for (int i = 0 ; i<this->getNumOfDimensions() ; i++) {
      file << cell.second.get()->coord[i] << " ";
    }
    file << "\n";
    //file << "cell_coordIndex: ";
    for (int i = 0 ; i<this->getNumOfDimensions() ; i++) {
      file << cell.second.get()->coordIndex[i] << " ";
    }
    file << "\n";
    file << cell.second.get()->coreCell << "\n";        //<< "cell_coreCell: "
    file << cell.second.get()->cellId << "\n";          // "cell_cellId: " <<
    file << cell.second.get()->cellParent << "\n";      //"cell_cellParent: " <<
    file << cell.second.get()->cellParentKey << "\n";   //"cell_cellParentKey: " <<
    file << cell.second.get()->clusterIndex << "\n";    //"cell_clusterIndex: " <<
    file << cell.second.get()->cellKey << "\n";         //"cell_cellKey: " <<
    file << cell.second.get()->points.size() << "\n";   //"cell_points_num: " <<
    //zoznam jej bodov
    file << "POINTS_START\n";
    for (auto &point : cell.second.get()->points) {
      //file << "point_coord: ";
      for (int i = 0 ; i < this->getNumOfDimensions() ; i++) {
        file << point.get()->coord[i] << " ";
      }
      file << "\n";
      file << point.get()->corePoint << "\n";         //"point_core_point: " <<
      file << point.get()->st << "\n";                //"point_process_status: " <<
      file << point.get()->gr << "\n";                //"point_group_status: " <<
      file << point.get()->clusterIndex << "\n";      //"point_cluster_index: " <<
    }
    file << "POINTS_END\nCELL_END";
  }

  file.close();
  std::cout << "File created.\n";
  return 0;
}

int Level::loadResultFromFile(std::string fileName, DataSet *dataInput) {

  std::ifstream file;
  file.open(fileName);

  std::string input;
  int numOfCells = 0;
  int numOfPoints = 0;

  if (file.is_open()) {
    cells.clear();
    levelGridSize = 0;

    getline(file, input);       //header
    getline(file, input);       //header
    getline(file, input);       //newline

    file >> input;        //dataset`s number of dimensions
    file >> numOfDimensions;
    file >> input;        //levelGridSize
    file >> levelGridSize;
    file >> input;        //cellNum
    file >> numOfCells;

    dataInput->coordBordersMax = new double[numOfDimensions];
    dataInput->coordBordersMin = new double[numOfDimensions];

    for (int i = 0 ; i<numOfDimensions ; i++) {
      file >> dataInput->getCoordBordersMin()[i];
    }
    for (int i = 0 ; i<numOfDimensions ; i++) {
      file >> dataInput->getCoordBordersMax()[i];
    }

    std::string mapId;
    for (int i = 0; i < numOfCells; i++) {
      file >> input;        //CELL START
      file >> mapId;

      //Cell *cell = new Cell();
      auto initCell = std::make_shared<Cell>();
      initCell->coord = new double[numOfDimensions];
      initCell->coordIndex = new int[numOfDimensions];
      //file >> input;
      file >> initCell->pointCounter;
      //file >> input;
      file >> initCell->cellGridSize;
      //file >> input;
      for (int j = 0 ; j<numOfDimensions ; j++) {
        file >> initCell->coord[j];
      }
      //file >> input;
      for (int j = 0 ; j<numOfDimensions ; j++) {
        file >> initCell->coordIndex[j];
      }
      //file >> input;
      file >> initCell->coreCell;
      //file >> input;
      file >> initCell->cellId;
      //file >> input;
      file >> initCell->cellParent;
      //file >> input;
      file >> initCell->cellParentKey;
      //file >> input;
      file >> initCell->clusterIndex;
      //file >> input;
      file >> initCell->cellKey;
      //file >> input;
      file >> numOfPoints;
      file >> input;    //POINTS START

      initCell.get()->points.clear();
      int groupSt, processSt;
      for (unsigned int j = 0; j < numOfPoints; j++) {
        std::shared_ptr<Point> p = std::make_shared<Point>(numOfDimensions);
        dataInput->getInputPoints().emplace_back(p);
        initCell.get()->points.emplace_back(p);
        //file >> input;
        for (int k = 0; k < this->getNumOfDimensions(); k++) {
          file >> initCell.get()->points.at(j).get()->coord[k];
        }
        //file >> input;
        file >> initCell.get()->points.at(j).get()->corePoint;
        //file >> input;
        file >> processSt;
        switch (processSt) {
        case 0:
          initCell.get()->points.at(j).get()->st = Point::visited;
          break;
        default:
          initCell.get()->points.at(j).get()->st = Point::unvisited;
        }
        //file >> input;
        file >> groupSt;
        switch (groupSt) {
        case 0:
          initCell.get()->points.at(j).get()->gr = Point::inCluster;
          break;
        case 1:
          initCell.get()->points.at(j).get()->gr = Point::noise;
          break;
        default:
          initCell.get()->points.at(j).get()->gr = Point::undecided;
        }
        //file >> input;
        file >> initCell.get()->points.at(j).get()->clusterIndex;
      }

      file >> input;    //POINTS END
      cells[mapId] = initCell;
    }
    getline(file, input);    //CELL END
  }
  file.close();
  std::cout << "File loaded.\n";
  return 0;
}

void Level::setNumOfDimensions(int dim) {
  numOfDimensions = dim;
}

int Level::getNumOfDimensions() {
  return numOfDimensions;
}

void Level::setLevelGridSize(double gridSize) {
  levelGridSize = gridSize;
}

double Level::getLevelGridSize() {
  return levelGridSize;
}

int Level::getNumOfCells() {
  return cells.size();
}

CellMap Level::getCells() {
  return cells;
}



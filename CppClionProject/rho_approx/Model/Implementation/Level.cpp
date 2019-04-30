#include <iostream>
#include <cmath>
#include <omp.h>
#include "../Level.h"
#include "../Cell.h"

//filter cells that are empty
void Level::evaluateCells(long &idCounter, std::vector<std::shared_ptr<Point>> &inputPoints){

    CellMap nonEmptyCells;

    //TODO paralelizovat
    for (auto &cell : cells) {
        if (cell.second->pointCounter > 0) {
            cell.second->cellId = idCounter;
            //mark core points and cells
            if(cell.second->pointCounter > MIN_PTS){
                cell.second->coreCell = true;
            }
            //checkCoreClusters(inputPoints);
            nonEmptyCells[cell.second->cellKey] = cell.second;
        }
        idCounter++;
    }

    //TODO preskumat ci treba, ci daco robi
    cells.clear();

    idCounter--;

    cells = nonEmptyCells;
}

std::shared_ptr<Cell> Level::findCell(std::shared_ptr<Point> &point){

    std::string pointKey = "";
    int index;

    for (int j = 0; j < DIMENSION; j++) {
        index = (int) floor(point->coord[j] / levelGridSize);
        pointKey += std::to_string(index);
        pointKey += ",";
    }

    if ( cells.find(pointKey) == cells.end() ) {
        return nullptr;
    } else {
        return cells.find(pointKey)->second;
    }
}


void Level::recursiveCellInit(int actualDim, double *actualCoord, int *actualCoordIndex, double *bordersMax, double *bordersMin){

    auto *localActualCoordIndex = new int [DIMENSION];
    auto *localActualCoord = new double [DIMENSION];
    for(int i = 0 ; i<DIMENSION ; i++){
        localActualCoordIndex[i] = actualCoordIndex[i];
        localActualCoord[i] = actualCoord[i];
    }

    if(actualDim == DIMENSION) {
        return;
    }

    if(actualDim == DIMENSION - 1){
        int i;
        int cellNumInDim = (int) ceil((bordersMax[actualDim] - bordersMin[actualDim]) / levelGridSize);
    #ifdef OPENMP
        #pragma omp parallel for schedule(static) private (i) shared(cellNumInDim)
    #endif
        for(i = 0 ; i < cellNumInDim ; i++){
            auto initCell = std::make_shared<Cell>();
            initCell->cellGridSize = levelGridSize;
            initCell->coreCell = false;
            initCell->pointCounter = 0;
            initCell->cellParent = -1;
            initCell->clusterIndex = -1;
            for(int j = 0 ; j<actualDim ; j++) {
                initCell->coord[j] = localActualCoord[j];
                initCell->coordIndex[j] = localActualCoordIndex[j];
            }
            initCell->coord[actualDim] = i*levelGridSize + bordersMin[actualDim];
            initCell->coordIndex[actualDim] = i;
        #ifdef OPENMP
            #pragma omp critical
        #endif
            cells.emplace_back(initCell);
        }
        delete[] localActualCoordIndex;
        delete[] localActualCoord;
        return;
    }

    while(localActualCoord[actualDim] < bordersMax[actualDim]){
        recursiveCellInit(actualDim + 1, localActualCoord, localActualCoordIndex, bordersMax, bordersMin);
        localActualCoord[actualDim]+= levelGridSize;
        localActualCoordIndex[actualDim]+= 1;
    }
    delete[] localActualCoordIndex;
    delete[] localActualCoord;
}


void Level::initFirstLevel(std::vector<std::shared_ptr<Point>> inputPoints, double *bordersMax, double *bordersMin){

    //trying to obtain n-dimensional cube (composed of first level cells)
    //iterating over whole space containing points
    unsigned int i, actualDim = 0;
    int cellNumInDim = (int) ceil((bordersMax[actualDim] - bordersMin[actualDim]) / levelGridSize);
    std::cout << "\ncell num in first dim: " << cellNumInDim << "\tgridSize: " << levelGridSize << "\n";
    std::cout << "max in first dim: " << bordersMax[actualDim]  << "\tmin in first dim: " << bordersMin[actualDim] << "\n";
    std::cout << "max in second dim: " << bordersMax[actualDim+1]  << "\tmin in second dim: " << bordersMin[actualDim+1] << "\n\n";


    double actualCoord[DIMENSION];
    int actualCoordIndex[DIMENSION];
    int cellNums[DIMENSION];
    int fillIndex = DIMENSION - 1;
    int fillIndexCounter = 0;

    long expectedCellNum = 1;
    for(i = 0 ; i<DIMENSION ; i++){
        cellNums[i] = (int) ceil((bordersMax[i] - bordersMin[i]) / levelGridSize);
        std::cout << "\n----- " << i << ". dim = " << cellNums[i] << " ";
        expectedCellNum *= (long) cellNums[i];
        actualCoord[i] = bordersMin[i];     //starting coordinates in every dimension set to min value
        actualCoordIndex[i] = 0;
    }
    std::cout << "\nExpected num of cells: " << expectedCellNum << "\n";


#ifdef POINT_SPECIFIC_SPACE_INIT

    for (i = 0; i < inputPoints.size(); i++) {
        auto point = inputPoints.at(i);
        auto cell = findCell(point);

        if(cell != nullptr){
            cell->pointCounter++;
            cell->points.emplace_back(point);
        }
        else{
            //create cell at right position
            auto initCell = std::make_shared<Cell>();
            initCell->cellGridSize = levelGridSize;
            initCell->coreCell = false;
            initCell->pointCounter = 0;
            initCell->cellParent = -1;
            initCell->clusterIndex = -1;
            for (int j = 0; j < DIMENSION; j++) {
                //set coords of cell start
                for(int k = 0 ; k<cellNums[j] ; k++) {
                    if (point->coord[j] >= (levelGridSize*k + bordersMin[j]) && point->coord[j] <= (levelGridSize*(k+1) + bordersMin[j])) {
                        initCell->coord[j] = (levelGridSize*k + bordersMin[j]);
                        initCell->coordIndex[j] = k;
                        break;
                    }
                }
            }
            initCell->cellKey = "";
            for (int index : initCell->coordIndex) {
                initCell->cellKey += std::to_string(index);
                initCell->cellKey += ",";
            }
            cells[initCell->cellKey] = initCell;

            initCell->pointCounter++;
            initCell->points.emplace_back(point);
        }

        pointCounter++;
    }
    std::cout << "\ninput size: " << inputPoints.size() << "\tpoints assigned: " << pointCounter << "\n\n\n";
#endif

#ifdef RECURSIVE_SPACE_INIT
    //PAMATOVO SMRTIACA REKURZIA
    actualCoord[actualDim] -= levelGridSize;
    for (i = 0; i < cellNumInDim; i++) {
        actualCoord[actualDim] += levelGridSize;
        actualCoordIndex[actualDim] = i;
        recursiveCellInit(actualDim + 1, actualCoord, actualCoordIndex, bordersMax, bordersMin);
    }
#endif

#ifdef LINEAR_SPACE_INIT

    while(true){

        for(int j = DIMENSION - 1 ; j>fillIndex ; j--){

            for(int k = 0 ; k<cellNums[j] ; k++) {
                actualCoord[j] = bordersMin[j] + k * levelGridSize;
                actualCoordIndex[j] = k;

                auto initCell = std::make_shared<Cell>();
                initCell->cellGridSize = levelGridSize;
                initCell->coreCell = false;
                initCell->pointCounter = 0;
                initCell->cellParent = -1;
                initCell->clusterIndex = -1;
                for (int l = 0; l < DIMENSION; l++) {
                    initCell->coord[l] = actualCoord[l];
                    initCell->coordIndex[l] = actualCoordIndex[l];
                }
                cells.emplace_back(initCell);
            }
            actualCoord[j] = bordersMin[j];
            actualCoordIndex[j] = 0;
        }
        fillIndexCounter++;
        actualCoord[fillIndex] = bordersMin[fillIndex] + fillIndexCounter*levelGridSize;
        actualCoordIndex[fillIndex] = fillIndexCounter;

        if(fillIndexCounter == cellNums[fillIndex]){
            actualCoord[fillIndex] = bordersMin[fillIndex];
            actualCoordIndex[fillIndex] = 0;
            fillIndexCounter = 0;
            fillIndex--;
        }
        if(fillIndex < 0){
            break;
        }
    }
#endif

}

void Level::checkCoreClusters(std::vector<std::shared_ptr<Point>> &input) {

    int counter = cells.size();
    int sum;
    std::vector<std::shared_ptr<Point>> neighbours;

    for (int i = 0; i < counter; i++) {
        if (cells[i]->pointCounter >= MIN_PTS && cells.at(
                static_cast<unsigned int>(i)).get()->cellGridSize <= 2 * sqrt(2) * EPS) {
            cells[i]->coreCell = true;
        }
        else {
            for (int j = 0; j < cells[i]->pointCounter; j++) {

                (cells[i]->points[j])->tagNeighbours(*(cells[i]->points[j]), input,
                                                                    neighbours, EPS);
                sum = neighbours.size();
                if (sum >= MIN_PTS) {
                    cells[i]->points[j]->corePoint = true;
                    cells[i]->coreCell = true;
                    break;
                }
                neighbours.clear();
            }
        }
    }
}

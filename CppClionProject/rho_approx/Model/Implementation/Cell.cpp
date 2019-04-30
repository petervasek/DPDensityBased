#include <iostream>
#include <cmath>
#include <limits>
#include "../Cell.h"
#include "../Level.h"
#include "../DataSet.h"


void Cell::addEdges(Level *specificLevel, double maxEps, DataSet input){

    int counter = specificLevel->cells.size();
    double distance;

    for(int i = 0 ; i<counter ; i++){
        for(int j = i+1 ; j<counter ; j++){
            distance = input.getCellDistance(specificLevel->cells.at(i), specificLevel->cells[j], maxEps);
            if(distance < maxEps){
                Edge *validEdge;
                validEdge = new Edge();
                validEdge->firstCell = specificLevel->cells[i];
                validEdge->secondCell = specificLevel->cells[j];
                specificLevel->cells[i]->edges.push_back(validEdge);
            }
        }
    }
}


void Cell::extendPoints(Level *lowerLevel) {

    long pointCounter = 0;

    for(auto &i : this->points) {
        int assignedFlag = 0;
        for (int j = static_cast<int>(lowerLevel->cells.size() - pow(2, DIMENSION));
             j < lowerLevel->cells.size(); j++) {
            int dimCounter = 0;
            for (int k = 0; k < DIMENSION; k++) {
                if (i->coord[k] >= lowerLevel->cells[j]->coord[k] &&
                    i->coord[k] <= lowerLevel->cells[j]->coord[k] + this->cellGridSize / 2.0) {
                    dimCounter++;
                }
            }
            if (dimCounter == DIMENSION) {
                assignedFlag++;
                lowerLevel->cells[j]->pointCounter++;
                lowerLevel->cells[j]->points.emplace_back(i);
                pointCounter++;
                continue;
            }
        }

        if (!assignedFlag) {
            int indexOfCellToAssign = 0;
            double minDistToCell = std::numeric_limits<double>::max();

            for (int j = static_cast<int>(lowerLevel->cells.size() - pow(2, DIMENSION)); j < lowerLevel->cells.size(); j++) {
                double dist = i.get()->getDistance(lowerLevel->cells[j].get());
                if(minDistToCell > dist){
                    minDistToCell = dist;
                    indexOfCellToAssign = j;
                }
            }
            lowerLevel->cells[indexOfCellToAssign]->pointCounter++;
            lowerLevel->cells[indexOfCellToAssign]->points.emplace_back(i);
            pointCounter++;
        }
    }
}

void Cell::recursiveExpandCell(Level *cellLevel, int actualDim, double *axis) {

    if(actualDim == 0) {
        return;
    }

    if(actualDim == 1) {
        std::shared_ptr<Cell> init1Cell(new Cell);
        init1Cell->cellGridSize = this->cellGridSize/2.0;
        init1Cell->coreCell = false;
        init1Cell->pointCounter = 0;
        for (int i = DIMENSION - 1; i >= 0; i--) {
            init1Cell->coord[i] = axis[i];
        }
        init1Cell->cellParent = this->cellId;
        cellLevel->cells.emplace_back(init1Cell);

        std::shared_ptr<Cell> init2Cell(new Cell);
        init2Cell->cellGridSize = this->cellGridSize/2.0;
        init2Cell->coreCell = false;
        init2Cell->pointCounter = 0;
        axis[actualDim-1] += this->cellGridSize/2.0;
        for (int i = DIMENSION - 1; i >= 0; i--) {
            init2Cell->coord[i] = axis[i];
        }
        init2Cell->cellParent = this->cellId;
        cellLevel->cells.emplace_back(init2Cell);
    }

    if (actualDim > 1) {
        recursiveExpandCell(cellLevel, actualDim-1, axis);
        axis[actualDim-1] += this->cellGridSize/2.0;
        recursiveExpandCell(cellLevel, actualDim-1, axis);
    }
}

void Cell::expandCell(Level *cellLevel) {

    double axis[DIMENSION];
    for (int i = 0; i < DIMENSION; i++)
        axis[i] = this->coord[i];

    recursiveExpandCell(cellLevel, DIMENSION, axis);
}

bool Cell::checkIfIsSameCluster(std::shared_ptr<Cell> targetCell) {

    for (auto &point : this->points) {
        for (auto &j : targetCell->points) {
            if(point.get()->getDistance(j.get()) < EPS){
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
*/
bool Cell::isCoreCell() {
    return coreCell;
}

std::string Cell::getKey(){
    return cellKey;
};

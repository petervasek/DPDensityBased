#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include <memory>
#include <omp.h>
#include "../DataSet.h"
#include "../Cluster.h"
#include "../Cell.h"
#include "../Level.h"
#include "../Edge.h"

void DataSet::getInputPoints(std::vector<std::shared_ptr<Point>> &inputPoints, double *bordersMax, double *bordersMin) {

    std::ifstream file;
    file.open(INPUT_FILE);

    //we need limit values in every dimension for future calculations
    for (int i = 0; i < DIMENSION; i++) {
        bordersMax[i] = std::numeric_limits<double>::min();
        bordersMin[i] = std::numeric_limits<double>::max();
    }

    if (file.is_open()) {


#ifdef SKIP_FIRST_LINE
        //skip first line with column names
        if(!file.eof()) {
            std::string columnNames;
            getline(file, columnNames);
        }
        else {
            file.close();
            return;
        }
#endif

        int pointIndex = 0;
        while (!file.eof()) {
            inputPoints.emplace_back(new Point);

            int i;
            for (i = 0; i < DIMENSION; i++) {
                file >> inputPoints[pointIndex]->coord[i];
            }

                for (i = 0; i < DIMENSION; i++) {
                    double coordValue = inputPoints[pointIndex]->coord[i];
                    if (coordValue > bordersMax[i])
                        bordersMax[i] = coordValue;
                    if (coordValue < bordersMin[i])
                        bordersMin[i] = coordValue;
                }
            inputPoints[pointIndex]->st = Point::unvisited;
            inputPoints[pointIndex]->gr = Point::undecided;
            inputPoints[pointIndex]->clusterIndex = -1;
            inputPoints[pointIndex]->corePoint = false;

            pointIndex++;
        }
    }
    file.close();
}

double DataSet::getPointDistance(Point *p1, Point *p2) {

    double distance = 0;

    for (int i = 0; i < DIMENSION; i++)
        distance += pow(p1->coord[i] - p2->coord[i], 2);
    distance = sqrt(distance);

    return distance;
}

double DataSet::getCellDistance(Cell *firstCell, Cell *secondCell, double maxEps) {

    double cellDistance;

    for (int i = 0; i < firstCell->pointCounter; i++)
        for (int j = 0; j < secondCell->pointCounter; j++) {
            cellDistance = getPointDistance(firstCell->points.at(i), secondCell->points[j]);
            if (cellDistance <= maxEps)
                return cellDistance;
        }

    return maxEps + 1;
}

//assigns points from input file to empty cells (n-cubes) that are filling n-dimensional space
void DataSet::assignPoints(std::vector<std::shared_ptr<Point>> &inputPoints, Level &firstLevel) {

    unsigned int i;

    for (i = 0; i < inputPoints.size(); i++) {
        auto point = inputPoints.at(i);
        auto cell = firstLevel.findCell(point);
        cell->pointCounter++;
        cell->points.emplace_back(point);
    #ifdef OPENMP
        #pragma omp critical
    #endif
        pointCounter++;
    #ifdef ASSIGN_MONITORING
        std::cout << pointCounter << "\n";
    #endif
    }
    std::cout << "\ninput size: " << inputPoints.size() << "\tpoints assigned: " << pointCounter << "\n\n\n";
}


void DataSet::unionCells(Edge *clusterConnect) {

    int root1, root2;
    root1 = clusterConnect->firstCell->cellParent;
    root2 = clusterConnect->secondCell->cellParent;

    if (root1 >= 0 && root2 >= 0)
        clusterConnect->secondCell->cellParent = root1;
}

void DataSet::connectClusters(Level *specificLevel) {

    for (int i = 0; i < specificLevel->cells.size(); i++) {
        for (int j = 0; j < specificLevel->cells[i]->edges.size(); j++) {
            unionCells(specificLevel->cells[i]->edges[j]);
        }
    }
}

void DataSet::fillClusters(Level *specificLevel, std::vector<Cluster> clusters) {
    std::vector<int> clusterIndexes[specificLevel->cells.size()];
    int clusterCounter = 0;
    Cluster *addCluster;
    for (int i = 0; i < specificLevel->cells.size(); i++) {
        clusterIndexes[specificLevel->cells[i]->cellParent].push_back(i);
    }

    //list indexes of cells
    for (int i = 0; i < specificLevel->cells.size(); i++) {
        if (clusterIndexes[i].size() > 0) {
            for (int j = 0; j < clusterIndexes[i].size(); j++)
                std::cout << clusterIndexes[i][j] << " ";
            std::cout << "\n";
        }
    }


    addCluster = new Cluster();
    clusters.push_back(*addCluster);
    std::cout << clusters.size() << "\n";

    for (int i = 0; i < specificLevel->cells.size(); i++) {
        if (clusterIndexes[i].size() > 0) {
            addCluster = new Cluster();
            clusters.push_back(*addCluster);
            //clusterCounter++;
            //for(int j = 0 ; j<clusterIndexes[i].size() ; j++){
            //    *clusters[clusterCounter-1].cells.push_back(specificLevel->cells[clusterIndexes[i][j]);
            //}
        }
    }
    std::cout << clusters.size() << "\n";
}

void DataSet::printAllClusters(std::vector<Cluster *> clusters) {
    for (int i = 0; i < clusters.size(); i++) {
        for (int j = 0; j < clusters[i]->cells.size(); j++) {
            int k = clusters[i]->cells[j]->points.size();
            std::cout << i + 1 << ". zhluk:";
            for (int l = 0; l < k; l++) {
                std::cout << "\n\t";
                for (int m = 0; m < DIMENSION; m++)
                    std::cout << clusters[i]->cells[j]->points[l]->coord[m] << " ";
            }
            std::cout << "\n";
        }
    }
}

void DataSet::printNoise(std::vector<Point> &input) {

    int count = 0;

    std::cout << "\nNoise:\n";
    for (int i = 0; i < (int) input.size(); i++) {
        if (input[i].gr == Point::noise) {
            count++;
            for (int j = 0; j < DIMENSION; j++)
                std::cout << input[i].coord[j] << " ";
            std::cout << "\n";
        }
    }
    if (count == 0)
        std::cout << "no noise\n";
}

void DataSet::getClusterOf(std::shared_ptr<Point> &p, std::vector<std::shared_ptr<Level>> allLevels, int *numOfClusters) {

    int pointsAddesToCluster = 0;
    std::vector<std::vector<int>> indexesOfCellsOnEachLevelToExplore;

    *numOfClusters = *numOfClusters + 1;

    for (int i = 0; i < allLevels.size(); i++) {
        if(indexesOfCellsOnEachLevelToExplore.empty()) {
            for (int j = 0; j < allLevels[i].get()->cells.size(); j++) {
                int neighboursWithRho = 0;
                int neighboursWithoutRho = 0;
                for (auto &point : allLevels[i].get()->cells[j].get()->points) {
                    double dist = p->getDistance(point.get());
                    if (dist < EPS) {
                        neighboursWithoutRho++;
                    }
                    if (dist < EPS *(1 + RHO)) {
                        neighboursWithRho++;
                    }
                }
                //whole cell is in same cluster as point, no need to look down its structure tree
                if (neighboursWithRho == allLevels[i].get()->cells[j].get()->points.size()) {
                    for (auto &point : allLevels[i].get()->cells[j].get()->points) {
                        if (point.get()->clusterIndex == -1) {
                            point.get()->clusterIndex = *numOfClusters;
                            pointsAddesToCluster++;
                        } else {
                            p.get()->clusterIndex = point.get()->clusterIndex;
                            return;
                        }
                    }
                    for (auto &edges : allLevels[i].get()->cells[j].get()->edges) {
                        edges.get()->firstCell->addCellToCluster(*numOfClusters);
                    }
                }
                indexesOfCellsOnEachLevelToExplore.emplace_back(std::vector<int>());
                if (neighboursWithRho < allLevels[i].get()->cells[j].get()->points.size() && neighboursWithRho > 0)
                    indexesOfCellsOnEachLevelToExplore[i].emplace_back(j);
            }
        }
        else {
            for (int j : indexesOfCellsOnEachLevelToExplore.at(static_cast<unsigned int>(i))) {
                int neighboursWithRho = 0;
                int neighboursWithoutRho = 0;
                for (auto &point : allLevels[i].get()->cells[j].get()->points) {
                    double dist = p->getDistance(point.get());
                    if (dist < EPS) {
                        neighboursWithoutRho++;
                    }
                    if (dist < EPS * (1 + RHO)) {
                        neighboursWithRho++;
                    }
                }
                //whole cell is in same cluster as point, no need to look down its structure tree
                if (neighboursWithRho == allLevels[i].get()->cells[j].get()->points.size()) {
                    for (auto &point : allLevels[i].get()->cells[j].get()->points) {
                        if (point.get()->clusterIndex == -1) {
                            point.get()->clusterIndex = *numOfClusters;
                            pointsAddesToCluster++;
                        } else {
                            p.get()->clusterIndex = point.get()->clusterIndex;
                            return;
                        }
                    }
                    for (auto &edges : allLevels[i].get()->cells[j].get()->edges) {
                        edges.get()->firstCell->addCellToCluster(*numOfClusters);
                    }
                }
                indexesOfCellsOnEachLevelToExplore.emplace_back(std::vector<int>());
                if (neighboursWithRho < allLevels[i].get()->cells[j].get()->points.size() && neighboursWithRho > 0)
                    indexesOfCellsOnEachLevelToExplore[i].emplace_back(j);
            }
        }
    }

    for(int i = 0 ; i<indexesOfCellsOnEachLevelToExplore.size() ; i++) {
        for (int j = 0; i < indexesOfCellsOnEachLevelToExplore[i].size(); j++) {
            Cell *c = allLevels[i].get()->cells[indexesOfCellsOnEachLevelToExplore[i][j]].get();
            for(int k = 0 ; k < c->points.size() ; k++){

            }
        }
    }

    if(pointsAddesToCluster == 0){
        *numOfClusters = *numOfClusters - 1;
    }
}
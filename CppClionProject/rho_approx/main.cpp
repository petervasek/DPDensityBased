#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include <memory>
#include <wtypesbase.h>
#include <omp.h>
#include <ctime>
#include "Model/Cluster.h"
#include "Model/Point.h"
#include "Model/Level.h"
#include "main.h"


int main(int argc, char* argv[]) {
    clock_t start = clock();
    DataSet input;
    std::vector<std::shared_ptr<Point>> inputPoints;
    std::unique_ptr<double> coordBordersMax(new double[DIMENSION]);
    std::unique_ptr<double> coordBordersMin(new double[DIMENSION]);

    //read input matrix
    input.getInputPoints(inputPoints, coordBordersMax.get(), coordBordersMin.get());


    //algorithm parameters
    int minPts;
    double maxEps, rho;

    //get attributes for algorithm calculations
    #ifdef MIN_PTS
        minPts = MIN_PTS;
    #endif
    #ifdef EPS
        maxEps = EPS;
    #endif
    #ifdef RHO
        rho = RHO;
    #endif

    #ifndef MIN_PTS
        std::cout << "Set minPts: ";
        std::cin >> minPts;
    #endif
    #ifndef EPS
        std::cout << "\n" << "Set maxEps: ";
        std::cin >> maxEps;
    #endif
    #ifndef RHO
        std::cout << "\n" << "Set value of rho: ";
        std::cin >> rho;
    #endif

    //DEPRECATED
    //creating structure for rho-approx-DBSCAN in O(n)
    double gridSize = maxEps / sqrt(DIMENSION);
    long cellIdCounter = 0;

    Level cellLevel;
    cellLevel.levelGridSize = gridSize;

    cellLevel.initFirstLevel(inputPoints, coordBordersMax.get(), coordBordersMin.get());

    std::cout << cellLevel.cells.size() << "\n";

    std::cout << "num of cells: " << cellLevel.cells.size() << "\n\nMaxs: ";


#ifndef POINT_SPECIFIC_SPACE_INIT
    input.assignPoints(inputPoints, cellLevel);
#endif


    std::cout << cellLevel.cells.size() << "\n";
    cellLevel.evaluateCells(cellIdCounter, inputPoints);
    std::cout << cellLevel.cells.size() << "\n";

    allLevels.push_back(std::make_shared<Level>(firstLevel));

    #ifdef PV_CELL_LEVEL
            for (auto &cell : cellLevel->cells) {
                for (double j : cell.get()->coord)
                    std::cout << j << " ";
                std::cout << cell->pointCounter << "\n";
            }
            std::cout << "\n" << firstLevel.cells.size() << "\n\n";

            std::cout << gridSize << " " << (maxEps * (1 + rho)) / sqrt(DIMENSION) * MIN_GRID_SIZE_MULTIPLIER;
    #endif

    //expand all first level cells/cubes to trees that have n-dim cells/cubes with size min gridSizeLimit as leafs
    unsigned int levelProcessed = 0;
    Level *actualLevel = &firstLevel;
    while(gridSize/2.0 > gridSizeLimit){
        auto *lowerLevel = new Level();
        lowerLevel->levelGridSize = gridSize/2.0;

        for (auto &cell : actualLevel->cells) {
            cell->expandCell(lowerLevel);
            cell->extendPoints(lowerLevel);
        }
        lowerLevel->evaluateCells(cellIdCounter, inputPoints);

        allLevels.push_back(std::make_shared<Level>(*lowerLevel));

#ifdef PV_CELLS_LOWER_LEVELS
            int numOfPointsAtLevel = 0;
            for (auto &cell : allLevels[allLevels.size()-1].get()->cells) {
                std::cout << "\ncell id: " << cell.get()->cellId << "\t\tcell parent id: " <<
                          cell.get()->cellParent << "\t\tcell grid size: " <<
                          cell.get()->cellGridSize << "\t\tnum of points: " <<
                          cell.get()->pointCounter << "\n";
                numOfPointsAtLevel += cell->pointCounter;
            }
            std::cout << gridSize << " " << gridSizeLimit << " " << numOfPointsAtLevel << "\n";
#endif

        levelProcessed++;
        actualLevel = lowerLevel;
        gridSize /= 2.0;
        //all levels created
    }


unsigned int i = 0;
for (auto &cell : cellLevel.cells) {
    i += cell.second.get()->points.size();
}

    //create edges between cells that belongs to same cluster
    for(int i = 0 ; i<allLevels.size() ; i++){
        for(int j = 0 ; j<allLevels.at(static_cast<unsigned int>(i)).get()->cells.size() ; j++){
            auto actualCell = allLevels.at(static_cast<unsigned int>(i)).get()->cells.at(static_cast<unsigned int>(j));
            if(actualCell.isCoreCell()) {
                for (int k = 0 ; k<allLevels.at(static_cast<unsigned int>(i)).get()->cells.size()-1 ; k++){
                    auto targetCell = allLevels.at(static_cast<unsigned int>(i)).get()->cells.at(static_cast<unsigned int>(k));
                    if(actualCell->checkIfIsSameCluster(targetCell)){
                        Edge *e = new Edge();
                        e->firstCell = actualCell;
                        e->secondCell = targetCell;
                        actualCell->edges.emplace_back(e);
                        targetCell->edges.emplace_back(e);
                    }
                }
            }
        }
    }

    #ifdef PV_NUMBERS_OF_EDGES
        for (auto &allLevel : allLevels) {
            for (auto &cell : allLevel.get()->cells) {
                std::cout << cell.get()->edges.size() << "\t";
            }
            std::cout << "\n";
        }
    #endif

    //result is gathered by querying for points not in any cluster
    int *numOfClusters = new int[1]{0};

    for (auto &inputPoint : inputPoints) {
        if(inputPoint->clusterIndex == -1){
            input.getClusterOf(inputPoint, allLevels, numOfClusters);
        }
    }

    int sum = 0;
    std::ofstream file;
    file.open(OUTPUT_FILE);
    for (auto &inputPoint : inputPoints) {
        file << inputPoint->toString();
        file << inputPoint->clusterIndex << "\n";
        std::cout << inputPoint->clusterIndex << "\n";
        if(inputPoint.get()->clusterIndex != -1) {
            sum++;
        }
    }
    file.close();


    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    std::cout << "\nTime elapsed: " << elapsed;
    return 0;
}
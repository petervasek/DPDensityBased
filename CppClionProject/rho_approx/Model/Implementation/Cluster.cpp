#include <iostream>
#include "../Cluster.h"
#include "../Point.h"


Cluster Cluster::expand(std::vector<Point*> &neighbours, Cluster &actualCluster, double maxEps, int minPoints, std::vector<Point> &input){

    int sum;
    int neighNum = neighbours.size();
    std::vector<point*> expandNeighbours;

    for(int i=0 ; i<neighNum ; i++){
        if(neighbours[i]->st == unvisited) {
            neighbours[i]->st = visited;
            neighbours[i]->gr = inCluster;
            actualCluster.points.push_back(neighbours[i]);

            expandNeighbours.clear();
            tagNeighbours(*neighbours[i], input, expandNeighbours, maxEps);
            sum = expandNeighbours.size();

            if(sum >= minPoints) {
                neighbours.insert(neighbours.end(), expandNeighbours.begin(), expandNeighbours.end());
                neighNum = neighbours.size();
            }
        }
    }

    return actualCluster;
}

std::string Cluster::printCluster(Cluster &actualCluster) {
    for (int i = 0; i < (int) actualCluster.cells.size(); i++) {
        for (int j = 0; j < actualCluster.cells[i]->points.size(); j++) {
            for (int k = 0; k < DIMENSION; k++)
                std::cout << actualCluster.cells[i]->points[j]->coord[k] << " ";

            if (actualCluster.cells[i]->points[j]->st == 0)
                std::cout << "visited, ";
            else
                std::cout << "unvisited, ";

            if (actualCluster.cells[i]->points[j]->gr == 0)
                std::cout << "inCluster";
            else if (actualCluster.cells[i]->points[j]->gr == 1)
                std::cout << "noise";
            else
                std::cout << "undecided";
            std::cout << "\n";
        }
    }
    getchar();
};
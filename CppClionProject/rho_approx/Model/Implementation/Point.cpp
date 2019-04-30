#include <cmath>
#include <sstream>
#include "../Point.h"
#include "../../main.h"
#include "../Cell.h"

Point::Point(){
    coord = new double[DIMENSION];
};


Point::~Point(){
    delete[] coord;
};

void Point::tagNeighbours(Point actual, std::vector<std::shared_ptr<Point>> &input, std::vector<std::shared_ptr<Point>> &neighbours, double maxEps){

    double dist;
    int i, j;

    for(i=0 ; i<(int)input.size() ; i++) {
        dist = 0;

        for (j=0 ; j<DIMENSION ; j++)
            dist += pow(actual.coord[j] - input[i]->coord[j], 2);

        dist = sqrt(dist);

        if(dist < maxEps) {
            neighbours.push_back(input[i]);
        }

        if(neighbours.size() >= MIN_PTS) {
            break;
        }
    }
}

std::string Point::toString() {
    std::string result;
    std::ostringstream s;

    for (int i = 0; i < DIMENSION; i++) {
        s << coord[i];
        s << " ";
    }
    result += s.str();
    //result += "\n";

    return result;
}

double Point::getDistance(Point *p) {

    double result = 0;

    //Euclidean distance
    for(int i = 0 ; i<DIMENSION ; i++) {
        result += pow(coord[i] - p->coord[i], 2);
    }

    return sqrt(result);
}

double Point::getDistance(Cell *c) {
    double result = 0;

    //Euclidean distance
    for(int i = 0 ; i<DIMENSION ; i++) {
        result += pow(coord[i] - c->coord[i], 2);
    }

    return sqrt(result);
}

bool Point::isCorePoint() {
    return corePoint;
}

/*
public:
    Point::Point(double coords[]){

    };

public:
    std::String getCoord(){

    };
*/
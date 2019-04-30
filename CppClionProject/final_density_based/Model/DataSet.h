#ifndef DP_DATASET_H
#define DP_DATASET_H

#include <vector>
#include <memory>
#include "Point.h"
#include "../main.h"

class Cluster;
class Cell;
class Level;
class Edge;

class DataSet {
    std::vector<std::shared_ptr<Point>> inputPoints;
    int numOfDimensions;

public:
    DataSet();
    ~DataSet();
    bool datasetHasHeader(std::string firstLine);
    int findOutNumOfDimensions(std::string firstLine);
    void loadInputPoints(std::string fileName);
    void printAllClusters(std::vector<Cluster*> clusters);
    void printNoise(std::vector<Point> &input);
    int getNumOfDimensions();
    void setNumOfDimensions(int num);
    virtual std::vector<std::shared_ptr<Point>> &getInputPoints();
    virtual double* getCoordBordersMax();
    virtual double* getCoordBordersMin();
    std::string getOutputName(std::string purpose, std::string inputFile, double eps, int minPts);

    double *coordBordersMax;
    double *coordBordersMin;
};

#endif //DP_DATASET_H

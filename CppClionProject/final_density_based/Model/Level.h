#ifndef DP_LEVEL_H
#define DP_LEVEL_H

#include <vector>
#include <map>
#include "Cell.h"

typedef std::map<std::string, std::shared_ptr<Cell>> CellMap;

class Level {
public:
    double levelGridSize;
    CellMap cells;
    int numOfDimensions;

public:
    Level();
    ~Level();
    std::shared_ptr<Cell> findCell(std::shared_ptr<Point> &point, double *bordersMin, int dim);
    void initFirstLevel(DataSet *input);
    int saveResultToFile(std::string fileName, DataSet *input);
    int loadResultFromFile(std::string fileName, DataSet *input);
    void setNumOfDimensions(int dim);
    int getNumOfDimensions();
    void setLevelGridSize(double gridSize);
    virtual double getLevelGridSize();
    virtual int getNumOfCells();
    virtual CellMap getCells();
};


#endif //DP_LEVEL_H

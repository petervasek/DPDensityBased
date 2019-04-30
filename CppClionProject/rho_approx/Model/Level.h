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

public:
    std::shared_ptr<Cell> findCell(std::shared_ptr<Point> &point);
    void recursiveCellInit(int actualDim, double *actualCoord, int *actualCoordIndex, double *bordersMax, double *bordersMin);
    void initFirstLevel(std::vector<std::shared_ptr<Point>> inputPoints, double *bordersMax, double *bordersMin);
    void evaluateCells(long &idCounter, std::vector<std::shared_ptr<Point>> &input);
    void checkCoreClusters(std::vector<std::shared_ptr<Point>> &input);
};


#endif //DP_LEVEL_H

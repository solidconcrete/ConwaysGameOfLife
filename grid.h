#ifndef LOGIC_H
#define LOGIC_H
#include <vector>
#include "cellStruct.h"

class grid
{


public:
    grid(int w, int h);
    grid();
    void setSize(int w, int h);
    bool checkState(int x, int y);
    void setCellAlive(int x, int y);
    void setCellDead(int x, int y);
    std::vector<cell>  getGrid();
    void makeStep();
    void decideCellFate(int x, int y);
    void loadDataToGrid(std::vector<cell> cells);
    void run();
    void startThread();

private:

    int width;
    int height;
//    std::vector<int> allCells;
    std::vector<cell> cellMap;
    std::vector<cell> tempCellMap;
    bool isStopped;
};

#endif // LOGIC_H

#ifndef LOGIC_H
#define LOGIC_H
#include <vector>
#include "cellStruct.h"

class grid
{
    friend class TestGui;

public:
    grid(int w, int h);
    grid();

    void changeSize(int w, int h);
    std::vector<cell>  getGrid();
    void makeStep();
    void setCellAlive(int x, int y);
    void setCellDead(int x, int y);
    void setEmpty();

private:

    int width;
    int height;
    std::vector<cell> cellMap;
    std::vector<cell> tempCellMap;

    bool decideCellFate(cell cellToCheck);
};

#endif // LOGIC_H

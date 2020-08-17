#include "grid.h"
#include <QDebug>

grid::grid(int w, int h)
{
    if (w < 0 || h < 0)
    {
        throw "Negative coordinates condition!";
    }
    this->width = w;
    this->height = h;

    cellMap.assign(width * height, cell());
}
grid::grid()
{

}
void grid::setGridEmpty()
{
    cellMap.assign(width * height, cell());
}
std::vector<cell> grid::getGrid()
{
    return cellMap;
}

void grid::setCellAlive(int x, int y)
{
    unsigned int cellCoordinates = y * width + x;
    if (x > width || y > height || x < 0 || y < 0)
    {
        throw "Such coordinates don't exist!";
    }

    if (cellMap[cellCoordinates].isAlive == 0)
    {
        cellMap[cellCoordinates].isAlive = 1;
    }

    //find offset, which we'll use to find neighbors
    int leftNeighbor, rightNeighbor, aboveNeighbor, belowNeighbor;
    if (x == 0)
    {
        leftNeighbor = width - 1;
    }
    else leftNeighbor = - 1;

    if (x == width - 1)
    {
        rightNeighbor = (width - 1) * (-1);
    }
    else
    {
        rightNeighbor = 1;
    }

    if (y == 0)
    {
        aboveNeighbor = height * width - (width - x) - x;
    }
    else
    {
        aboveNeighbor = -width;
    }

    if (y == height - 1)
    {
        belowNeighbor = (height * width - width) * (-1);
    }
    else
    {
        belowNeighbor = width;
    }

// increment current cell's neighbors' "alive neighbors" field
    cellMap[cellCoordinates + aboveNeighbor].aliveNeighbors ++;
    cellMap[cellCoordinates + aboveNeighbor + leftNeighbor].aliveNeighbors ++;
    cellMap[cellCoordinates + aboveNeighbor + rightNeighbor].aliveNeighbors ++;

    cellMap[cellCoordinates + belowNeighbor].aliveNeighbors ++;
    cellMap[cellCoordinates + belowNeighbor + leftNeighbor].aliveNeighbors ++;
    cellMap[cellCoordinates + belowNeighbor + rightNeighbor].aliveNeighbors ++;

    cellMap[cellCoordinates + leftNeighbor].aliveNeighbors ++;
    cellMap[cellCoordinates + rightNeighbor].aliveNeighbors ++;


}

void grid::setCellDead(int x, int y)
{
    if (x > width || y > height || x < 0 || y < 0)
    {
        throw "Such coordinates don't exist!";
    }

    unsigned int cellCoordinates = y * width + x;

    if (cellMap[cellCoordinates].isAlive == 1)
    {
        cellMap[cellCoordinates].isAlive = 0;
    }


    int leftNeighbor, rightNeighbor, aboveNeighbor, belowNeighbor;
    if (x == 0)
    {
        leftNeighbor = width - 1;
    }
    else leftNeighbor = - 1;

    if (x == width - 1)
    {
        rightNeighbor = (width - 1) * (-1);
    }
    else
    {
        rightNeighbor = 1;
    }

    if (y == 0)
    {
        aboveNeighbor = height * width - (width - x) - x;
    }
    else
    {
        aboveNeighbor = - width;
    }

    if (y == height - 1)
    {
        belowNeighbor = (height * width - width) * (-1);
    }
    else
    {
        belowNeighbor = width;
    }


    cellMap[cellCoordinates + aboveNeighbor].aliveNeighbors --;
    cellMap[cellCoordinates + aboveNeighbor + leftNeighbor].aliveNeighbors --;
    cellMap[cellCoordinates + aboveNeighbor + rightNeighbor].aliveNeighbors --;

    cellMap[cellCoordinates + belowNeighbor].aliveNeighbors --;
    cellMap[cellCoordinates + belowNeighbor + leftNeighbor].aliveNeighbors --;
    cellMap[cellCoordinates + belowNeighbor + rightNeighbor].aliveNeighbors --;

    cellMap[cellCoordinates + leftNeighbor].aliveNeighbors --;
    cellMap[cellCoordinates + rightNeighbor].aliveNeighbors --;
}

void grid::makeStep()
{
    bool cellFate;
    tempCellMap = cellMap;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            cellFate = decideCellFate(tempCellMap.at(y * width + x));

            //if cell is alive and should be dead - kill it
            if (cellFate && !tempCellMap.at(y * width + x).isAlive)
            {
                setCellAlive(x, y);
            }
            //if cell is dead and should be alive - resurrect it
            if (!cellFate && tempCellMap.at(y * width + x).isAlive)
            {
                setCellDead(x, y);
            }

        }
    }
}

//decide whether the cell should be alive or dead
bool grid::decideCellFate(cell cellToCheck)
{
    if ((!cellToCheck.isAlive && cellToCheck.aliveNeighbors == 3))
    {
        return true;
    }

    if (cellToCheck.isAlive && (cellToCheck.aliveNeighbors == 2 || cellToCheck.aliveNeighbors == 3))
    {
        return true;
    }
    return false;
}

void grid::changeSize(int newWidth, int newHeight)
{
    QDebug deb = qDebug();
    if (newWidth < 0 || newHeight < 0)
    {
        throw "Negative coordinates condition!";
    }

    int smallerWidth;
    int smallerHeight;

    if (newWidth < width)
    {
        smallerWidth = newWidth;
    }
    else smallerWidth = width;
    if (newHeight < height)
    {
        smallerHeight = newHeight;
    }
    else smallerHeight = height;

    int oldWidth = width;

    std::vector<cell> oldMap = cellMap;
    cellMap.assign(newWidth * newHeight, cell());

    this->width = newWidth;
    this->height = newHeight;

    for(int row = 0; row < smallerHeight; row ++)
    {
        for (int col = 0; col < smallerWidth; col ++)
        {
            if (oldMap[row * oldWidth + col].isAlive)
            {
                setCellAlive(col, row);
            }
        }
    }
}

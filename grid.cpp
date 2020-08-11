#include "grid.h"
#include <QDebug>
#include <thread>
#include <chrono>
#include <vector>

grid::grid(int w, int h)
{
    if (w < 0 || h < 0)
    {
        throw "Negative coordinates condition!";
    }
    this->width = w;
    this->height = h;

    cellMap.assign(width * height, cell(0, 0));
}
grid::grid()
{

}
void grid::setSize(int newWidth, int newHeight)
{
    QDebug deb = qDebug();
    if (newWidth < 0 || newHeight < 0)
    {
        throw "Negative coordinates condition!";
    }

    int widthToCheck;
    int heightToCheck;

    if (newWidth < width)
    {
        widthToCheck = newWidth;
    }
    else widthToCheck = width;
    if (newHeight < height)
    {
        heightToCheck = newHeight;
    }
    else heightToCheck = height;

    int oldWidth = width;

    std::vector<cell> oldMap = cellMap;
    cellMap.assign(newWidth * newHeight, cell(0,0));
    this->width = newWidth;
    this->height = newHeight;

    for(int row = 0; row < heightToCheck; row ++)
    {
        for (int col = 0; col < widthToCheck; col ++)
        {
            if (checkState(col, row, oldMap, oldWidth))
            {
                setCellAlive(col, row);
            }
        }
    }
}

void grid::setEmpty()
{
    cellMap.assign(width * height, cell(0, 0));
}

bool grid::checkState(int x, int y, std::vector<cell> map, int w)
{
    if (x > width || y > height || x < 0 || y < 0)
    {
        throw "Such coordinates don't exist!";
    }
    return map.at(y * w + x).isAlive;
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
        aboveNeighbor = height * (width - 1);
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
        aboveNeighbor = height * (width - 1);
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
    tempCellMap = cellMap;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            decideCellFate(x, y);
        }
    }
}

void grid::decideCellFate(int x, int y)
{
    int index = y * width + x;
    if ((!tempCellMap.at(index).isAlive && tempCellMap.at(index).aliveNeighbors == 3))
    {
        setCellAlive(x, y);
    }
    if (tempCellMap.at(index).isAlive && (tempCellMap.at(index).aliveNeighbors < 2 || tempCellMap.at(index).aliveNeighbors > 3))
    {
        setCellDead(x, y);
    }
}

std::vector<cell> grid::getGrid()
{
    return cellMap;
}

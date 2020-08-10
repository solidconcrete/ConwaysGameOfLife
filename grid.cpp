#include "grid.h"
#include <QDebug>
#include <thread>
#include <chrono>

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
void grid::setSize(int w, int h)
{
    if (w < 0 || h < 0)
    {
        throw "Negative coordinates condition!";
    }
    this->width = w;
    this->height = h;

    cellMap.assign(width * height, cell(0, 0));
}
void grid::loadDataToGrid(std::vector<cell> cells)
{

}

//void grid::run()
//{
//    while(1)
//    {
//        if (!isStopped)
//        {
//            for( int i = 0; i < 100; i++)
//            {
//                qDebug() << "thread running, $ cycle" << i;

//                std::this_thread::sleep_for(std::chrono::milliseconds(100));
//            }
//            isStopped = true;
//        }
//    }
//}

void grid::startThread()
{
    isStopped = false;
}

bool grid::checkState(int x, int y)
{
    if (x > width || y > height || x < 0 || y < 0)
    {
        throw "Such coordinates don't exist!";
    }
    return cellMap.at(y * width + x).isAlive;
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

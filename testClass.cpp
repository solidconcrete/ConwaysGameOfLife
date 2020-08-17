#include <QtWidgets>
#include <QtTest/QtTest>
#include <grid.h>
#include <assert.h>
#include <exception>
#include "cellStruct.h"
class testClass: public QObject
{
    Q_OBJECT
private:
//    Dialog d;
private slots:
    void testCellConstructorIsAlive();
    void testCellConstructorAliveNeighbors();
    void testGridConstructor();
    void testChangeSize();
    void testNegativeWidthChangeSize();
    void testAliveCellShouldBeAlive();
    void testAliveCellShouldBeDead();
    void testDeadCellShouldBeDead();
    void testDeadCellShouldBeAlive();
    void testSetCellAlive();
};

void testClass::testAliveCellShouldBeAlive()
{
    cell cellToTest;
    grid g;
    int aliveNeighbors = 3;
    bool isAlive = 1;

    //set cell alive and has three neighbors
    cellToTest.isAlive = isAlive;
    cellToTest.aliveNeighbors = aliveNeighbors;

    bool cellFate = g.decideCellFate(cellToTest);

    //cell fate should be true since it has 3 neighbors
    QVERIFY(cellFate);
}

void testClass::testAliveCellShouldBeDead()
{
    cell cellToTest;
    grid g;
    int aliveNeighbors = 5;
    bool isAlive = 1;

    //set cell to be alive and have 5 neighbors
    cellToTest.isAlive = isAlive;
    cellToTest.aliveNeighbors = aliveNeighbors;

    bool cellFate = g.decideCellFate(cellToTest);

    //cell fate should be false since it has too many neighbors
    QVERIFY(!cellFate);
}

void testClass::testDeadCellShouldBeDead()
{
    cell cellToTest;
    grid g;
    int aliveNeighbors = 5;
    bool isAlive = 0;

    //set cell to be dead and have 5 neighbors
    cellToTest.isAlive = isAlive;
    cellToTest.aliveNeighbors = aliveNeighbors;

    bool cellFate = g.decideCellFate(cellToTest);

    //cell fate should be false since it has too many neighbors
    QVERIFY(!cellFate);
}

void testClass::testDeadCellShouldBeAlive()
{
    cell cellToTest;
    grid g;
    int aliveNeighbors = 3;
    bool isAlive = 0;

    //set cell dead and has 3 neighbors
    cellToTest.isAlive = isAlive;
    cellToTest.aliveNeighbors = aliveNeighbors;

    bool cellFate = g.decideCellFate(cellToTest);

    //cell fate should be true since it has 3 neighbors
    QVERIFY(cellFate);
}

void testClass::testCellConstructorIsAlive()
{
    // create cell
    cell c = cell();

    //get cell's isAlive value
    bool isAlive = c.isAlive;

    //newly created cell's isAlive value should be = 0
    QCOMPARE(0, isAlive);
}

void testClass::testCellConstructorAliveNeighbors()
{
    // create cell
    cell c = cell();

    //get cell's isAlive value
    int aliveNeighbors = c.aliveNeighbors;

    //newly created cell's isAlive value should be = 0
    QCOMPARE(0, aliveNeighbors);
}

void testClass::testGridConstructor()
{
    grid g;
    int w = 10, h = 5;

    //create grid which would have w width and h height
    g = grid(w, h);

    //get grid size
    int gridSize = g.getGrid().size();

    //grid should have w*h cells
    QCOMPARE(w*h, gridSize);
}

void testClass::testChangeSize()
{
    grid g;
    int w = 10, h = 5;
    int newW = 6, newH = 3;

    //create grid which would have w width and h height
    g = grid(w, h);

    //change grid size
    g.changeSize(newW, newH);

    //get new grid size
    int newSize = g.getGrid().size();

    //now the grid should have w*h cells
    QCOMPARE(newW * newH, newSize);
}

void testClass::testNegativeWidthChangeSize()
{
    grid g;
    int w = 10, h = 5;
    int newW = -6, newH = 26;

    bool isExceptionCatched = false;

    //create grid which would have w width and h height
    g = grid(w, h);

    try
    {
        //change grid size to have negative width
        g.changeSize(newW, newH);
    }
    catch (const char* msg)
    {
        //if exception is thwown, remember it
        isExceptionCatched = true;
    }

    //check whether the exception was thrown
    QVERIFY(isExceptionCatched);
}

void testClass::testSetCellAlive()
{
    int w = 10, h = 5;
    grid g = grid(w, h);

    int x = 0, y = 0;

    g.setCellAlive(x, y);

    std::vector<cell> cellMap = g.getGrid();

    int index = y * w + x;

    bool isCellAlive = cellMap.at(index).isAlive;

    QVERIFY(isCellAlive);
}


//uncomment this line and comment main class to run tests
//QTEST_MAIN(testClass)
#include "testClass.moc"

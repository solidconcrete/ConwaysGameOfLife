#ifndef CELLSTRUCT_H
#define CELLSTRUCT_H
struct cell
{
    int aliveNeighbors;
    bool isAlive;
    cell ():
        aliveNeighbors(0),
        isAlive(0)
    {}
};
#endif // CELLSTRUCT_H

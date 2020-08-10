#ifndef CELLSTRUCT_H
#define CELLSTRUCT_H
struct cell
{
    int aliveNeighbors;
    bool isAlive;
    cell (int neighbors, bool alive):
        aliveNeighbors(neighbors), isAlive(alive) {}
};
#endif // CELLSTRUCT_H

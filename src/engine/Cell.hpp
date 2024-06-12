#ifndef CELL_H
#define CELL_H

#include <list>
#include "VerletObject.hpp"

class Cell {
  public:
    void addNeighbour(Cell*);
    void addObject(VerletObject*);
    void clearObjects();
    void checkCollisions();

  private:
    std::list<Cell*> neighbours;
    std::list<VerletObject*> container;
};

#endif


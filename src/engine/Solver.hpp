#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

#include "Cell.hpp"

class Solver {
  public:
    Solver(std::vector<VerletObject>& objects);
    ~Solver();

    void update(float dt);

    const Cell* getCellAt(int x, int y) const;

  private:
    std::vector<VerletObject>& objects;
    std::vector<Cell> cells;
    sf::Vector2f gravity{0.f, 1000.f};

  private:
    void solveCollisions();
};

#endif


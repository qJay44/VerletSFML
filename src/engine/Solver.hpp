#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

#include "Cell.hpp"
#include "ThreadPool.hpp"

class Solver {
  public:
    Solver(std::vector<VerletObject>& objects);
    ~Solver();

    void update(float dt);
    void switchGravity();
    void reset();

  private:
    std::vector<VerletObject>& objects;
    std::vector<Cell> cells;
    sf::Vector2f gravity{0.f, 80.f};
    sf::Vector2f attractor{WIDTH * 0.5f, HEIGHT * 0.5f};
    ThreadPool tp;
    bool useAttraction = false;

  private:
    void solveCollisions();
};

#endif


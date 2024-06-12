#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

#include "ThreadPool.h"
#include "VerletObject.hpp"
#include "quadtree.hpp"

class Solver {
  public:
    Solver(std::vector<VerletObject>& objects);
    ~Solver();

    void setQuadTree(qt::Node* qt);
    void update(float dt);

  private:
    std::vector<VerletObject>& objects;
    sf::Vector2f gravity{0.f, 1000.f};
    qt::Node* quadtree = nullptr;
    ThreadPool tp;

  private:
    void solveCollisions();
    void solveCollisionsThreaded(int begin, int end);
};

#endif


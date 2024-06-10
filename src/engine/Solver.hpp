#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

#include "VerletObject.hpp"
#include "quadtree.hpp"

class Solver {
  public:
    Solver(std::vector<VerletObject>& objects);

    void setQuadTree(qt::Node* qt);
    void update(float dt);

  private:
    std::vector<VerletObject>& objects;
    sf::Vector2f gravity{0.f, 1000.f};
    qt::Node* quadtree = nullptr;

  private:
    void applyGravity();
    void applyConstraint();
    void solveCollisions();
    void updatePositions(float dt);
};

#endif


#include "Cell.hpp"

void solveCollision(VerletObject* obj1, VerletObject* obj2) {
  sf::Vector2f collisionAxis = obj1->positionCurrent - obj2->positionCurrent;
  float dist = magnitude(collisionAxis);
  float minDist = obj1->radius + obj2->radius;

  if (dist < minDist) {
    constexpr float responseCoef = 0.75f;
    sf::Vector2f n = collisionAxis / (dist + 0.01f);
    float delta = 0.5f * (minDist - dist) * responseCoef;
    obj1->positionCurrent += delta * n;
    obj2->positionCurrent -= delta * n;
  }
}

void Cell::addNeighbour(Cell* rhs) {
  neighbours.push_back(rhs);
}

void Cell::addObject(VerletObject* obj) {
  container.push_back(obj);
}

void Cell::clearObjects() {
  container.clear();
}

void Cell::checkCollisions() {
  // Check within neighbours
  for (Cell* rhs : neighbours)
    for (VerletObject* obj1 : container)
      for (VerletObject* obj2 : rhs->container)
        if (obj1 != obj2)
          solveCollision(obj1, obj2);

  // Check within myself (this)
  for (VerletObject* obj1 : container) {
    for (VerletObject* obj2 : container) {
      if (obj1 != obj2)
        solveCollision(obj1, obj2);
    }
  }
}


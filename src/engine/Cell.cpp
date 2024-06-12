#include <cmath>

#include "Cell.hpp"

void solveCollision(VerletObject* obj1, VerletObject* obj2) {
  sf::Vector2f collisionAxis = obj1->positionCurrent - obj2->positionCurrent;
  float distSq = collisionAxis.x * collisionAxis.x + collisionAxis.y * collisionAxis.y;
  float minDist = obj1->radius + obj2->radius;
  float minDistSq = minDist * minDist;

  if (distSq < minDistSq) {
    constexpr float responseCoef = 0.75f;

    float dist = sqrtf(distSq);
    sf::Vector2f n = collisionAxis / (dist + 0.001f);
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
  for (const Cell* rhs : neighbours)
    for (VerletObject* obj1 : container)
      for (VerletObject* obj2 : rhs->container)
        solveCollision(obj1, obj2);

  // Check within myself (this)
  for (VerletObject* obj1 : container)
    for (VerletObject* obj2 : container)
      if (obj1 != obj2)
        solveCollision(obj1, obj2);
}


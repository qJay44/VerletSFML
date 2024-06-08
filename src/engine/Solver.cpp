#include "Solver.hpp"

Solver::Solver(std::vector<VerletObject>& objects) : objects(objects) {}

void Solver::update(float dt) {
  applyGravity();
  applyConstraint();
  solveCollisions();
  updatePositions(dt);
}

void Solver::applyGravity() {
  for (VerletObject& obj : objects)
    obj.accelerate(gravity);
}

void Solver::applyConstraint() {
  for (VerletObject& obj : objects) {
    if (obj.positionCurrent.y - obj.radius <= 0.f) obj.positionCurrent.y = obj.radius; // Top
    if (obj.positionCurrent.y + obj.radius >= HEIGHT) obj.positionCurrent.y = HEIGHT - obj.radius; // Bottom
    if (obj.positionCurrent.x - obj.radius <= 0.f) obj.positionCurrent.x = obj.radius; // Left
    if (obj.positionCurrent.x + obj.radius >= WIDTH) obj.positionCurrent.x = WIDTH - obj.radius; // Right
  }
}

void Solver::solveCollisions() {
  for (int i = 0; i < objects.size(); i++) {
    VerletObject& obj1 = objects[i];
    for (int j = i + 1; j < objects.size(); j++) {
      VerletObject& obj2 = objects[j];
      sf::Vector2f collisionAxis = obj1.positionCurrent - obj2.positionCurrent;
      float dist = magnitude(collisionAxis);
      float minDist = obj1.radius + obj2.radius;

      if (dist < minDist) {
        sf::Vector2f n = collisionAxis / dist;
        float delta = minDist - dist;
        obj1.positionCurrent += 0.5f * delta * n;
        obj2.positionCurrent -= 0.5f * delta * n;
      }
    }
  }
}

void Solver::updatePositions(float dt) {
  for (VerletObject& obj : objects)
    obj.updatePosition(dt);
}


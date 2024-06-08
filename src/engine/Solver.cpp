#include "Solver.hpp"

Solver::Solver(std::vector<VerletObject>& objects) : objects(objects) {}

void Solver::update(float dt) {
  applyGravity();
  updatePositions(dt);
  applyConstraint();

  /* for (VerletObject& obj : objects) { */
  /*   obj.accelerate(gravity); */

  /*   if (obj.positionCurrent.y - obj.radius <= 0.f) obj.positionCurrent.y = obj.radius; // Top */
  /*   if (obj.positionCurrent.y + obj.radius >= HEIGHT) obj.positionCurrent.y = HEIGHT - obj.radius; // Bottom */
  /*   if (obj.positionCurrent.x - obj.radius <= 0.f) obj.positionCurrent.x = obj.radius; // Left */
  /*   if (obj.positionCurrent.x + obj.radius >= WIDTH) obj.positionCurrent.x = WIDTH - obj.radius; // Right */

  /*   obj.updatePosition(dt); */
  /* } */
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

void Solver::updatePositions(float dt) {
  for (VerletObject& obj : objects)
    obj.updatePosition(dt);
}


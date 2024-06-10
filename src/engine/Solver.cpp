#include "Solver.hpp"

Solver::Solver(std::vector<VerletObject>& objects)
  : objects(objects) {}

void Solver::setQuadTree(qt::Node* qt) {
  quadtree = qt;
}

void Solver::update(float dt) {
  float dtSub = dt / SUB_STEPS;
  for (int i = 0; i < SUB_STEPS; i++) {
    applyGravity();
    applyConstraint();
    updatePositions(dtSub);
    solveCollisions();
  }
}

void Solver::applyGravity() {
  for (VerletObject& obj : objects)
    obj.accelerate(gravity);
}

void Solver::applyConstraint() {
  for (VerletObject& obj : objects) {
    // Vertical
    if (obj.positionCurrent.y - obj.radius <= 0.f) obj.positionCurrent.y = obj.radius;
    else if (obj.positionCurrent.y + obj.radius >= HEIGHT) obj.positionCurrent.y = HEIGHT - obj.radius;

    // Horizontal
    if (obj.positionCurrent.x - obj.radius <= 0.f) obj.positionCurrent.x = obj.radius;
    else if (obj.positionCurrent.x + obj.radius >= WIDTH) obj.positionCurrent.x = WIDTH - obj.radius;
  }
}

void Solver::solveCollisions() {
  for (int i = 0; i < objects.size(); i++) {
    VerletObject& obj1 = objects[i];
    std::list<void*> found;
    quadtree->query(found, {obj1.positionCurrent.x, obj1.positionCurrent.y, obj1.radius * 2.f, obj1.radius * 2.f});

    for (void* dataObj : found) {
      VerletObject* obj2 = (VerletObject*) dataObj;
      if (&obj1 != obj2) {
        sf::Vector2f collisionAxis = obj1.positionCurrent - obj2->positionCurrent;
        float dist = magnitude(collisionAxis);
        float minDist = obj1.radius + obj2->radius;

        if (dist < minDist) {
          constexpr float responseCoef = 0.75f;
          sf::Vector2f n = collisionAxis / (dist + 0.01f);
          float delta = 0.5f * (minDist - dist) * responseCoef;
          obj1.positionCurrent += delta * n;
          obj2->positionCurrent -= delta * n;
        }
      }
    }
  }
}

void Solver::updatePositions(float dt) {
  for (VerletObject& obj : objects) {
    obj.updatePosition(dt);
    quadtree->insert({obj.positionCurrent.x, obj.positionCurrent.y, (void*)&obj});
  }
}


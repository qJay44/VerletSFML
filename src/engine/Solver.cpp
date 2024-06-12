#include "Solver.hpp"

Solver::Solver(std::vector<VerletObject>& objects)
  : objects(objects) {
    tp.start();
  }

Solver::~Solver() {
  tp.stop();
}

void Solver::setQuadTree(qt::Node* qt) {
  quadtree = qt;
}

void Solver::update(float dt) {
  float dtSub = dt / SUB_STEPS;
  for (int i = 0; i < SUB_STEPS; i++) {
    for (VerletObject& obj : objects) {
      // Apply gravity
      obj.accelerate(gravity);

      // Apply vertical constraint
      if (obj.positionCurrent.y - obj.radius <= 0.f) obj.positionCurrent.y = obj.radius;
      else if (obj.positionCurrent.y + obj.radius >= HEIGHT) obj.positionCurrent.y = HEIGHT - obj.radius;

      // Apply horizontal constraint
      if (obj.positionCurrent.x - obj.radius <= 0.f) obj.positionCurrent.x = obj.radius;
      else if (obj.positionCurrent.x + obj.radius >= WIDTH) obj.positionCurrent.x = WIDTH - obj.radius;

      // Update position
      obj.updatePosition(dtSub);
      quadtree->insert({obj.positionCurrent.x, obj.positionCurrent.y, (void*)&obj});
    }

    solveCollisions();
  }
}
void Solver::solveCollisions() {
  int tpSize = tp.size();
  int slice = objects.size() / tpSize;

  for (int i = 0; i < tpSize; i++) {
    int begin = i * slice;
    int end = begin + slice;

    if (i == tpSize - 1)
      end += objects.size() % tpSize;

    tp.queueJob([this, begin, end] {solveCollisionsThreaded(begin, end);});
  }
  tp.waitForCompletion();
}

void Solver::solveCollisionsThreaded(int begin, int end) {
  for (int i = begin; i < end; i++) {
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


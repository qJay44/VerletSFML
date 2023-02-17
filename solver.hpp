#pragma once
#include <cmath>
#include <vector>
#include "verlet_object.hpp"

class Solver {
  public:
    void addObject(VerletObject obj) {
      objects.push_back(obj);
    }

    void setConstraintPos(vec2 pos) {
      constraintPos = pos;
    }

    void setConstraintRadius(float r) {
      constraintRadius = r;
    }

    void update(float dt) {
      applyGravity();
      applyConstraint();
      updatePositions(dt);
    }

    std::vector<VerletObject> getObjects() const {
      return objects;
    }

  private:
    std::vector<VerletObject> objects;
    vec2 gravity = { 0.f, 1000.f };
    vec2 constraintPos;
    float constraintRadius;

    void applyGravity() {
      for (VerletObject &obj : objects)
        obj.accelerate(gravity);
    }

    void updatePositions(float dt) {
      for (VerletObject &obj : objects)
        obj.updatePosition(dt);
    }

    void applyConstraint() {
      for (VerletObject &obj : objects) {
        vec2 to_obj = constraintPos - obj.position_current;
        float dist = sqrt(to_obj.x * to_obj.x + to_obj.y * to_obj.y);
        if (dist > (constraintRadius - obj.radius)) {
          vec2 n = to_obj / dist;
          obj.position_current = constraintPos - n * (constraintRadius - obj.radius);
        }
      }
    }
};


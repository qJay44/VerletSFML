#pragma once
#include <cmath>
#include <stdint.h>
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
      const uint32_t sub_steps = 2;
      const float sub_dt = dt / (float) sub_steps;

      for (uint32_t i(sub_steps); i--;) {
        applyGravity();
        checkCollisions();
        applyConstraint();
        updatePositions(sub_dt);
      }
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

    void checkCollisions() {
      for (uint32_t i(0); i < objects.size(); ++i) {
        VerletObject &object_1 = objects[i];

        for (uint32_t j(i + 1); j < objects.size(); ++j) {
          VerletObject &object_2 = objects[j];
          const vec2 collision_axis = object_1.position_current - object_2.position_current;
          const float dist = sqrt(collision_axis.x * collision_axis.x + collision_axis.y * collision_axis.y);
          const float min_dist = object_1.radius + object_2.radius;

          if (dist < min_dist) {
            const vec2 n = collision_axis / dist;
            const float delta = min_dist - dist;

            object_1.position_current += n * delta * 0.5f;
            object_2.position_current -= n * delta * 0.5f;
          }
        }
      }
    }
};


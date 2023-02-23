#pragma once
#include "utils.hpp"
#include "verlet_object.hpp"
#include <cmath>

struct Link {
  VerletObject &obj1;
  VerletObject &obj2;
  float target_dist;

  void apply() {
    const vec2 axis = obj1.position_current - obj2.position_current;
    const float dist = sqrt(axis.x * axis.x + axis.y * axis.y);
    const vec2 n = axis / dist;
    const float delta = target_dist - dist;
    obj1.position_current += n * 0.5f * delta;
    obj2.position_current -= n * 0.5f * delta;
  }
};


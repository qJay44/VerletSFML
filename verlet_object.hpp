#pragma once
#include "utils.hpp"

struct VerletObject {
  vec2 position_current;
  vec2 position_old;
  vec2 acceleration;
  float radius = 10.f;

  void updatePosition(float dt) {
    const vec2 velocity = position_current - position_old;

    position_old = position_current;
    position_current = position_current + velocity + acceleration * dt * dt;
    acceleration = {};
  }

  void accelerate(vec2 acc) {
    acceleration += acc;
  }
};


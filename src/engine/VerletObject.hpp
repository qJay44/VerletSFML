#ifndef VERLET_H
#define VERLET_H

struct VerletObject {
  sf::Vector2f positionCurrent{};
  sf::Vector2f positionOld{};
  sf::Vector2f acceleration{};

  float radius;
  sf::Color color;

  void updatePosition(float dt) {
    sf::Vector2f velocity = positionCurrent - positionOld;
    positionOld = positionCurrent;
    positionCurrent += velocity + acceleration * dt * dt;
    acceleration = {};
  }

  void accelerate(sf::Vector2f acc) {
    acceleration += acc;
  }
};

#endif


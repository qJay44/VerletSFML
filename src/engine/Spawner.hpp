#ifndef SPAWNER_H
#define SPAWNER_H

#include "VerletObject.hpp"

class Spawner {
  public:
    Spawner(std::vector<VerletObject>& objects, sf::VertexArray& vertices, sf::Vector2f texSize, int count);
    bool add(float dt); // Returns true if added last element

  private:
    std::vector<VerletObject>& objects;
    sf::VertexArray& vertices;

    const sf::Vector2f texSize;
    const int goal = 0;

    int current = 0;
    int hue = 0;
    float angle = 0.f;
    float angleDir = 1.f; // Defines wherenever to increase (1) or decrease (-1) radians
};

#endif


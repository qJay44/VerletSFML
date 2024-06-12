#ifndef SPAWNER_H
#define SPAWNER_H

#include "VerletObject.hpp"

class Spawner {
  public:
    Spawner(std::vector<VerletObject>& objects, sf::VertexArray& vertices, sf::Vector2f texSize);
    void add(int lines, float dt);

  private:
    std::vector<VerletObject>& objects;
    sf::VertexArray& vertices;

    const sf::Vector2f texSize;

    int current = 0;
    float hue = 0.f;
};

#endif


#ifndef SPAWNER_H
#define SPAWNER_H

#include "VerletObject.hpp"

class Spawner {
  public:
    Spawner();
    Spawner(std::vector<VerletObject>* objects, sf::VertexArray* vertices, sf::Vector2f texSize);

    void add(int amount);
    void reset();

  private:
    std::vector<VerletObject>* objects;
    sf::VertexArray* vertices;
    sf::Vector2f texSize;
    float colorStep = 0.f;
    int colorDir = 1;
};

#endif


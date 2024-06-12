#include <cmath>

#include "Spawner.hpp"

Spawner::Spawner(std::vector<VerletObject>& objects, sf::VertexArray& vertices, sf::Vector2f texSize, int count)
  : objects(objects), vertices(vertices), texSize(texSize), goal(count) {}

bool Spawner::add(float dt) {
  constexpr float r = 1.f;
  constexpr float angle = 0.f;

  for (int i = 0; i < 4 && current++ < goal; i++) {
    sf::Vector2f pos{r, r + r * i * 2.f};
    sf::Vector2f radPos{cosf(angle), sinf(angle)};
    sf::Vector2f dir(radPos / magnitude(radPos));
    sf::Vector2f acc{dir.x * pos.x, dir.y * pos.y};

    VerletObject obj{pos + acc, pos, {0.f, 0.f}, r, hsv2rgb(hue++, 1.f, 1.f, 255.f)};
    objects.push_back(obj);

    sf::Vertex topLeft;
    sf::Vertex topRight;
    sf::Vertex bottomRight;
    sf::Vertex bottomLeft;

    topLeft.position     = {pos.x - r, pos.y - r};
    topRight.position    = {pos.x + r, pos.y - r};
    bottomRight.position = {pos.x + r, pos.y + r};
    bottomLeft.position  = {pos.x - r, pos.y + r};

    topLeft.color     = obj.color;
    topRight.color    = obj.color;
    bottomRight.color = obj.color;
    bottomLeft.color  = obj.color;

    topLeft.texCoords     = {0.f, 0.f};
    topRight.texCoords    = {texSize.x, 0.f};
    bottomRight.texCoords = {texSize.x, texSize.y};
    bottomLeft.texCoords  = {0.f, texSize.y};

    vertices.append(topLeft);
    vertices.append(topRight);
    vertices.append(bottomRight);
    vertices.append(bottomLeft);
  }

  return current == goal;
}


#include <cmath>

#include "Spawner.hpp"

Spawner::Spawner(std::vector<VerletObject>& objects, sf::VertexArray& vertices, sf::Vector2f texSize, int count)
  : objects(objects), vertices(vertices), texSize(texSize), goal(count) {}

bool Spawner::add(float dt) {
  constexpr int minRadius = 3;
  constexpr int maxRadius = 7;
  static const sf::Vector2f accInit{maxRadius, maxRadius};
  float r = std::rand() % (maxRadius - minRadius + 1) + minRadius;

  sf::Vector2f pos{maxRadius, maxRadius};
  sf::Vector2f radPos{cosf(angle), sinf(angle)};
  sf::Vector2f dir(radPos / magnitude(radPos));
  sf::Vector2f acc{dir.x * accInit.x, dir.y * accInit.y};

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

  angle += SPAWNER_ANGLE_STEP * angleDir * dt;
  if (angle <= 0.f || angle >= PI_2) angleDir *= -1.f;

  return ++current == goal;
}


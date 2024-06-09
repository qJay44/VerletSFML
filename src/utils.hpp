#ifndef UTILS_H
#define UTILS_H

#include <cmath>

sf::Color hsv2rgb(int hue, float sat, float val, const float d);

inline float magnitude(const sf::Vector2f& v) {
  return sqrtf(v.x * v.x + v.y * v.y);
}

#endif


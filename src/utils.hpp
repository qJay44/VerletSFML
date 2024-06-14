#ifndef UTILS_H
#define UTILS_H

#include <cmath>

inline float magnitude(const sf::Vector2f& v) {
  return sqrtf(v.x * v.x + v.y * v.y);
}

#endif


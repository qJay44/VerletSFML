#include "utils.hpp"
#include <cmath>

sf::Color hsv2rgb(int hue, float sat, float val, const float d) {
  hue %= 360;
  while (hue < 0) hue += 360;

  if (sat < 0.f) sat = 0.f;
  else if (sat > 1.f) sat = 1.f;

  if (val < 0.f) val = 0.f;
  else if (val > 1.f) val = 1.f;

  const int h = hue / 60;
  float f = float(hue) / 60 - h;
  float p = val * (1.f - sat);
  float q = val * (1.f - sat * f);
  float t = val * (1.f - sat * (1 - f));

  switch (h) {
    default:
    case 6: return sf::Color(val * 255, t * 255, p * 255, d);
    case 1: return sf::Color(q * 255, val * 255, p * 255, d);
    case 2: return sf::Color(p * 255, val * 255, t * 255, d);
    case 3: return sf::Color(p * 255, q * 255, val * 255, d);
    case 4: return sf::Color(t * 255, p * 255, val * 255, d);
    case 5: return sf::Color(val * 255, p * 255, q * 255, d);
  }
}

float magnitude(const sf::Vector2f& v) {
  return sqrtf(v.x * v.x + v.y * v.y);
}


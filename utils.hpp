#pragma once

struct vec2 {
  float x, y;

  vec2 operator+(const vec2 &v) {
    vec2 vec;
    vec.x = this->x + v.x;
    vec.y = this->y + v.y;

    return vec;
  };

  vec2 operator+(const float &val) {
    vec2 vec;
    vec.x = this->x + val;
    vec.y = this->y + val;

    return vec;
  };

  vec2 operator+=(const vec2 &v) {
    this->x += v.x;
    this->y += v.y;

    return *this;
  };

  vec2 operator-(const vec2 &v) {
    vec2 vec;
    vec.x = this->x - v.x;
    vec.y = this->y - v.y;

    return vec;
  };

  vec2 operator-(const float &val) const {
    vec2 vec;
    vec.x = this->x - val;
    vec.y = this->y - val;

    return vec;
  };

  vec2 operator-=(const vec2 &v) {
    this->x -= v.x;
    this->y -= v.y;

    return *this;
  };

  vec2 operator*(const vec2 &v) const {
    vec2 vec;
    vec.x = this->x * v.x;
    vec.y = this->y * v.y;

    return vec;
  };

  vec2 operator*(const float &val) const {
    vec2 vec;
    vec.x = this->x * val;
    vec.y = this->y * val;

    return vec;
  };

  vec2 operator/(const float &val) const {
    vec2 vec;
    vec.x = this->x / val;
    vec.y = this->y / val;

    return vec;
  };
};


#pragma once

struct Point {
  int x;
  int y;

  Point operator+(const Point& other) const { return Point {x + other.x, y + other.y}; }
};

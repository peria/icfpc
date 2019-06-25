#pragma once

struct Point {
  int x;
  int y;

  Point operator+(const Point& other) const {
    return Point{x + other.x, y + other.y};
  }
  Point operator-(const Point& other) const {
    return Point{x - other.x, y - other.y};
  }
  Point& operator+=(const Point& other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  bool operator==(const Point& other) { return x == other.x && y == other.y; }
  bool operator!=(const Point& other) { return !((*this) == other); }
};

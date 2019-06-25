#include "wrapper.h"

Wrapper::Wrapper(const Point& p, int id): pos(p), index(id) {
  manipulators.emplace_back(Point {1, 1});
  manipulators.emplace_back(Point {1, 0});
  manipulators.emplace_back(Point {1, -1});
}

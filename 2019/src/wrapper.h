#pragma once

#include <vector>

#include "point.h"

class Wrapper {
 public:
  Wrapper(const Point& p, int index);

  Point pos;
  // Relative position of manipulators from the wrapper.
  std::vector<Point> manipulators;
  const int index;
};

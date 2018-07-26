#pragma once

#include "corrdinate.h"

struct Region {

  int dimension() const { return (c1.x == c2.y) + (c1.y == c2.y) + (c1.z == c2.z); }

  Coordinate c1;
  Coordinate c2;
};

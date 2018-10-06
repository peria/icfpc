#include "coordinate.h"

std::ostream& operator<<(std::ostream& ost, const Coordinate& c) {
  ost << "(" << c.x << ", " << c.y << ", " << c.z << ")";
  return ost;
}

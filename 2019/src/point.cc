#include "point.h"

std::ostream& operator<<(std::ostream& os, const Point& p) {
  os << '(' << p.x << ',' << p.y << ')';
  return os;
}

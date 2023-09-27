#include "color.h"

#include <ostream>

std::ostream& operator<<(std::ostream& os, const Color& c) {
  return os << "(" << (int)c[0] << ", " << (int)c[1] << ", " << (int)c[2]
            << ", " << (int)c[3] << ")";
}

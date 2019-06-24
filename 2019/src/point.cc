#include "point.h"

#include <cassert>
#include <cstdlib>
#include <vector>

namespace {

using Polygon = std::vector<Point>;
using Polygons = std::vector<Polygon>;

}  // namespace

template<>
Point parse(const char*& p) {
  assert (*p == '(');
  int x = std::strtol(++p, const_cast<char**>(&p), 10);
  assert (*p == ',');
  int y = std::strtol(++p, const_cast<char**>(&p), 10);
  assert (*p == ')');
  ++p;
  return {x, y};
}

template<>
Polygon parse(const char*& p) {
  Polygon polygon;
  polygon.emplace_back(parse<Point>(p));
  while (*p == ',') {
    polygon.emplace_back(parse<Point>(++p));
  }
  return polygon;
}

template<>
Polygons parse(const char*& p) {
  Polygons polygons;
  polygons.emplace_back(parse<Polygon>(p));
  while (*p == ';') {
    polygons.emplace_back(parse<Polygon>(++p));
  }
  return polygons;
}

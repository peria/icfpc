#include "map.h"

#include <cassert>
#include <string>
#include <vector>
#include <fstream>

#include "booster.h"
#include "point.h"

namespace {

using Polygon = std::vector<Point>;
using Polygons = std::vector<Polygon>;

}  // namespace

Map::Map(const std::string& desc_file) {
  std::string desc;
  {
    std::ifstream ifs(desc_file);
    ifs >> desc;
  }
  const char* p = desc.data();
  Polygon map_polygon = parse<Polygon>(p);
  assert (*p == '#');
  Point init_point = parse<Point>(++p);
  assert (*p == '#');
  Polygons obstacles = parse<Polygons>(++p);
  assert (*p == '#');
  std::vector<Booster> boosters = parse<std::vector<Booster>>(++p);
}

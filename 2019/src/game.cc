#include "game.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "booster.h"
#include "desc_parser.h"
#include "map.h"
#include "point.h"

namespace {

using Polygon = std::vector<Point>;
using Polygons = std::vector<Polygon>;

// Fills inside of |polygon| with |value|, on |data|.
void fillPolygon(const Polygon& polygon, CellType value, Map& map) {
  std::vector<std::vector<int>> vertical_lines(map.kWidth);
  for (int i = 0; i < polygon.size(); ++i) {
    int j = (i + 1) % polygon.size();
    const Point& p0 = polygon[i];
    const Point& p1 = polygon[j];
    if (p0.x != p1.x) {
      int xmin = std::min(p0.x, p1.x);
      int xmax = std::max(p0.x, p1.x);
      for (int x = xmin; x < xmax; ++x) {
        vertical_lines[x].push_back(p0.y);
      }
    }
  }
  for (int x = 0; x < map.kWidth; ++x) {
    auto& xlines = vertical_lines[x];
    assert (xlines.size() % 2 == 0);
    sort(xlines.begin(), xlines.end());
    for (int i = 0; i < xlines.size(); i += 2) {
      for (int y = xlines[i]; y < xlines[i + 1]; ++y) {
        map(x, y) = value;
      }
    }
  }
}

}  // namespace

Game::Game(const std::string& desc, const std::string& buy) {
  const char* ptr = desc.data();
  Polygon map_polygon = parse<Polygon>(ptr);
  assert (*ptr == '#');
  Point init_point = parse<Point>(++ptr);
  assert (*ptr == '#');
  Polygons obstacles = parse<Polygons>(++ptr);
  assert (*ptr == '#');
  std::vector<BoosterPos> boosters = parse<std::vector<BoosterPos>>(++ptr);

  int height = 0;
  int width = 0;
  for (auto& p : map_polygon) {
    width = std::max(width, p.x);
    height = std::max(height, p.y);
  }
  map = Map(width, height, CellType::kObstacle);
  fillPolygon(map_polygon, CellType::kNonWrapped, map);
  for (auto& obst : obstacles) {
    fillPolygon(obst, CellType::kObstacle, map);
  }
}

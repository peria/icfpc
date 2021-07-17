#pragma once

#include <set>
#include <utility>
#include <vector>

#include "base.h"
#include "bonus.h"

using Edge = std::pair<int, int>;

struct Problem {
  Problem(const char* filename);

  // Returns true if d^2=`d2` is allowed for the original d^2=`orig`.
  bool isAllowedD2(Integer orig, Integer d2) const;
  static Integer getD2(const Point& u, const Point& v);

  std::vector<Point> hole;
  std::vector<Point> vertices;
  std::vector<Edge> edges;
  Integer epsilon;
  std::vector<Bonus> bonuses;

  std::vector<std::set<int32>> vertices_next_to;

 private:
  void parseJsonFile(const char* filename);
};

inline bool Problem::isAllowedD2(Integer orig, Integer d2) const {
  return ((1000000 - epsilon) * orig <= 1000000 * d2) &&
         (1000000 * d2 <= (1000000 + epsilon) * orig);
}

inline Integer Problem::getD2(const Point& u, const Point& v) {
  Point d = u - v;
  return d.x * d.x + d.y * d.y;
}

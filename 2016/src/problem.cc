#include "problem.h"

#include <glog/logging.h>

#include <iostream>

#include "base.h"
#include "polygon.h"
#include "vertex.h"
using namespace std;

const Solution Problem::solve() {
  if (silhouette.isAsIs()) {
    cerr << "This is an as-is problem\n";
    return solveAsIs();
  }
  if (silhouette.size() == 1) {
    cerr << "Wrap with a rectangle\n";
    return solveWrap();
  }
  return Solution();
}

const Solution Problem::solveAsIs() {
  DCHECK_EQ(1, silhouette.polygons.size());
  DCHECK_EQ(4, silhouette.polygons[0].vertexs.size());

  Solution solution;

  solution.sources.push_back({0, 0});
  solution.sources.push_back({1, 0});
  solution.sources.push_back({1, 1});
  solution.sources.push_back({0, 1});

  solution.facets.push_back({0, 1, 2, 3});

  solution.destinations = silhouette.polygons[0].vertexs;

  return solution;
}

const Solution Problem::solveWrap() {
  Polygon polygon = silhouette.polygons[0];
  Solution normal = solveWrapCore(polygon);

  // See https://oeis.org/A001652. Very near values to 1/sqrt(2)
  static const Fraction kCos(1235216565974040, 1746860020068409);
  static const Fraction kSin(1235216565974041, 1746860020068409);
  // rotate 45 degree
  for (int64 i = 0; i < polygon.size(); ++i) {
    Vertex& v = polygon.vertexs[i];
    polygon.vertexs[i] = {v.x * kCos - v.y * kSin, v.x * kSin + v.y * kCos};
  }
  Solution rotated = solveWrapCore(polygon);

  if (rotated.area() < normal.area()) {
    // rotate -45 degre
    for (int64 i = 0; i < rotated.destinations.size(); ++i) {
      Vertex& v = rotated.destinations[i];
      rotated.destinations[i] = {v.x * kCos + v.y * kSin, -v.x * kSin + v.y * kCos};
    }
    return rotated;
  }
  return normal;
}

const Solution Problem::solveWrapCore(const Polygon& polygon) {
  Fraction xbase, ybase;
  vector<Fraction> xs = getPoints(polygon.vertexs, xbase, 0);
  vector<Fraction> ys = getPoints(polygon.vertexs, ybase, 1);

  Solution solution;
  int64 id_y = 0;
  for (const auto& y : ys) {
    int64 id_x = 0;
    for (const auto& x : xs) {
      solution.sources.push_back({x, y});

      Fraction to_x = xbase + xs[id_x % 2];
      Fraction to_y = ybase + ys[id_y % 2];
      if (id_x == xs.size() - 1) {
        Fraction rem = 1 - xs[id_x - 1];
        if (id_x % 2)
          to_x = xbase + rem;
        else
          to_x = xbase + xs[1] - rem;
      }
      if (id_y == ys.size() - 1) {
        Fraction rem = 1 - ys[id_y - 1];
        if (id_y % 2)
          to_y = ybase + rem;
        else
          to_y = ybase + ys[1] - rem;
      }
      solution.destinations.push_back({to_x, to_y});

      ++id_x;
    }
    ++id_y;
  }

  // facets
  for (int iy = 0; iy < ys.size() - 1; ++iy) {
    for (int ix = 0; ix < xs.size() - 1; ++ix) {
      int i0 = iy * xs.size() + ix, i1 = i0 + 1;
      int i2 = (iy + 1) * xs.size() + ix, i3 = i2 + 1;
      solution.facets.push_back({i0, i1, i3, i2});
    }
  }

  return solution;
}

std::vector<Fraction> Problem::getPoints(const std::vector<Vertex>& vertexs, Fraction& base, int index) {
  vector<Fraction> ps;
  if (index == 0) {
    for (const auto& v : vertexs)
      ps.push_back(v.x);
  } else {
    for (const auto& v : vertexs)
      ps.push_back(v.y);
  }

  Fraction pmin = ps[0];
  Fraction pmax = ps[1];
  for (const Fraction& p : ps) {
    pmin = min(p, pmin);
    pmax = max(p, pmax);
  }
  Fraction width = pmax - pmin;
  if (width > 1)
    width = 1;
  base = (pmax + pmin - width) / 2;

  ps.clear();
  for (Fraction x = 0; x <= 1; x += width) {
    ps.push_back(x);
  }
  if (ps.back() != 1)
    ps.push_back(1);

  return ps;
}

istream& operator>>(istream& is, Problem& problem) {
  is >> problem.silhouette >> problem.skeleton;
  return is;
}

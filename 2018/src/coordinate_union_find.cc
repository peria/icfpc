#include "coordinate_union_find.h"

#include <unordered_set>

#include "coordinate.h"

namespace {

const ND kDDs[] = {
    {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1},
};

}  // namespace

void CoordinateUnionFind::Register(const Coordinate& c) {
  parent[c] = c;
  size[c] = 1;
}

void CoordinateUnionFind::Unregister(const Coordinate& c) {
  parent.erase(c);
  size.erase(c);
}

Coordinate CoordinateUnionFind::Find(Coordinate c) {
  if (parent[c] != c)
    parent[c] = Find(parent[c]);
  return parent[c];
}

void CoordinateUnionFind::Union(Coordinate a, Coordinate b) {
  if (parent.find(a) == parent.end())
    return;
  if (parent.find(b) == parent.end())
    return;

  a = Find(a);
  b = Find(b);
  if (a == b)
    return;

  if (size[a] > size[b])
    std::swap(a, b);

  parent[b] = a;
  size[a] += size[b];
}

void CoordinateUnionFind::Clustering() {
  std::unordered_set<Coordinate, Coordinate::Hash> coords;
  for (auto& c : parent)
    coords.insert(c.first);
  for (auto& c : coords) {
    for (const ND& dd : kDDs) {
      Coordinate d(c + dd);
      if (coords.count(d))
        Union(c, d);
    }
  }
}

Coordinate CoordinateUnionFind::GetClose(const Coordinate& c) {
  Coordinate ret(-1, -1, -1);
  int dist = 1000000;
  for (auto& p : parent) {
    int d = (c - p.first).mLen();
    if (d < dist) {
      dist = d;
      ret = p.first;
    }
  }
  return ret;
}

CoordinateSet CoordinateUnionFind::GetCluster(Coordinate c) {
  CoordinateSet ret;
  c = Find(c);
  for (auto& p : parent) {
    if (Find(p.first) == c)
      ret.insert(p.first);
  }
  for (auto& p : ret) {
    parent.erase(p);
    size.erase(p);
  }

  return std::move(ret);
}

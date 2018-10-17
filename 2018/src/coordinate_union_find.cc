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

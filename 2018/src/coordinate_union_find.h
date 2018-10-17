#pragma once

#include <unordered_map>

#include "coordinate.h"

// A Coordinate-Coordinate union find.
struct CoordinateUnionFind {
  void Register(const Coordinate& c);

  Coordinate Find(Coordinate c);
  void Union(Coordinate a, Coordinate b);
  void Clustering();

  std::unordered_map<Coordinate, Coordinate, Coordinate::Hash> parent;
  std::unordered_map<Coordinate, int, Coordinate::Hash> size;
};

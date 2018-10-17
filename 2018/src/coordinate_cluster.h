#pragma once

#include "coordinate.h"

struct CoordinateCluster {
  void Register(const Coordinate& c);
  void Unregister(const Coordinate& c);

  Coordinate Find(Coordinate c);
  void Union(Coordinate a, Coordinate b);
  void Clustering();

  Coordinate GetClose(const Coordinate& c);
  CoordinateSet GetCluster(Coordinate c);

  CoordinateMap<Coordinate> parent;
  CoordinateMap<int> size;
};

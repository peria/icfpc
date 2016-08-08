#pragma once

#include <iostream>
#include <vector>

#include "base.h"
#include "polygon.h"

struct Silhouette {
  bool isAsIs();

  size_t size() const { return polygons.size(); }
  std::vector<Polygon> polygons;
};

std::istream& operator>>(std::istream& is, Silhouette& silhouette);

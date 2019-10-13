#include "polygon.h"

#include <iostream>

Coordinate::Coordinate(std::istream& is) {
  char c;
  is >> x >> c >> y;
  // CHECK_EQ(c, ',');
}

Polygon::Polygon(std::istream& is) {
  int num_vertex;
  is >> num_vertex;
  for (int i = 0; i < num_vertex; ++i)
    vertexs_.emplace_back(is);
}

Line::Line(std::istream& is) {
  vertexs_.emplace_back(is);
  vertexs_.emplace_back(is);
}


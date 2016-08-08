#pragma once

#include <iostream>

#include "base.h"
#include "vertex.h"

struct Polygon {
  Fraction width() const;
  Fraction height() const;

  Vertex edge(int id) const;

  size_t size() const { return this->vertexs.size(); }
  Vertex& operator[](int i) { return vertexs[i]; }
  std::vector<Vertex> vertexs;
};

std::istream& operator>>(std::istream& is, Polygon& polygon);

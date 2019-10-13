#pragma once

#include <istream>
#include <vector>

#include <gmpxx.h>

struct Coordinate {
 public:
  Coordinate(std::istream& is);
  ~Coordinate() = default;

  mpq_class x;
  mpq_class y;
};

class Polygon {
 public:
  Polygon(std::istream& is);
  ~Polygon() = default;

 private:
  std::vector<Coordinate> vertexs_;
};

class Line {
 public:
  Line(std::istream& is);
  ~Line() = default;

 private:
  std::vector<Coordinate> vertexs_;
};


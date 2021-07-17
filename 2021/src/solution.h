#pragma once

#include <vector>

#include "base.h"

struct Solution {
  Solution() = default;
  Solution(const std::vector<Point>& v) : vertices(v) {}
  ~Solution() {}
  void saveFile(const char* filepath);

  std::vector<Point> vertices;
  // TODO: obtained bonus
  // TODO: used bonus
};
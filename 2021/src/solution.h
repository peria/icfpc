#pragma once

#include <vector>

#include "base.h"
#include "geo.h"
#include "problem.h"

struct Solution {
  explicit Solution(const Problem& p);
  ~Solution() {}
  void saveFile(const char* filepath);

  std::vector<Point> vertices;
  const Problem& problem;
  // TODO: obtained bonus
  // TODO: used bonus
};
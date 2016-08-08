#pragma once

#include <iostream>
#include <vector>

#include "base.h"
#include "vertex.h"

struct Solution {
  Fraction area() const;
  Fraction width() const;
  Fraction height() const;

  std::vector<Vertex> sources;
  std::vector<std::vector<int>> facets;
  std::vector<Vertex> destinations;
};

std::ostream& operator<<(std::ostream& os, const Solution& solution);

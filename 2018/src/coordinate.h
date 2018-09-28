#pragma once

#include <cmath>

#include "base.h"

struct Coordinate {
  Coordinate(const Coordinate&) = default;

  int mLen() const { return std::abs(x) + std::abs(y) + std::abs(z); }
  int cLen() const {
    return std::max(std::abs(x), std::max(std::abs(y), std::abs(z)));
  }

  bool isLinear() const {
    return ((x | y) == 0 || (y | z) == 0 || (z | x) == 0) && (x | y | z);
  }
  bool isSLD() const { return isLinear() && cLen() <= 5; }
  bool isLLD() const { return isLinear() && cLen() <= 15; }

  bool isNCD() const { return 0 < mLen() && mLen() <= 2 && cLen() == 1; }
  bool isFCD() const { return 0 < cLen() && cLen() <= 30; }

  int hash() const { return (((x << 8) + y) << 8) + z; }

  int x = 0;
  int y = 0;
  int z = 0;
};

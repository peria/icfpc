#pragma once

#include <iostream>
#include <utility>
#include <vector>

#include "base.h"

using Pair = std::pair<int64, int64>;

struct Skeleton {
  std::vector<Pair> lines;
};

std::istream& operator>>(std::istream& is, Skeleton& skeleton);

#pragma once

#include <vector>

#include "matrix.h"
#include "nanobot.h"
#include "trace.h"

struct State {
  enum class Harmonics { kHigh, kLow };

  bool isWellFormed() const;

  int64 energy = 0;
  Harmonics harmonics;
  Matrix matrix;
  std::vector<Nanobot> bots;
  Trace trace;
};

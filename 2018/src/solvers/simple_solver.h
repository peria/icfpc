#pragma once

#include <string>
#include "solver.h"

class SimpleSolver : public Solver {
  Trace solve(const Matrix& src, const Matrix& dst) final;
  const char* name() final { return "simple"; }
};

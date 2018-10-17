#pragma once

#include <string>

#include "solver.h"

// SimpleSolver puts every Full voxel from ground. Do not run in parallel.
class SimpleSolver : public Solver {
  Trace solve(const Matrix& src, const Matrix& dst) final;
  const char* name() final { return "simple"; }
};

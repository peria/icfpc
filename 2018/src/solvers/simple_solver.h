#pragma once

#include <string>

#include "solver.h"

// SimpleSolver puts every Full voxel from ground. Do not run in parallel.
struct SimpleSolver : public Solver {
  virtual ~SimpleSolver() override {}

  Trace solve(const Matrix& src, const Matrix& dst) final;
  const char* name() final { return "simple"; }
};

#pragma once

#include <string>
#include "solver.h"

// HorizonSolver works as following.
// 1. scan horizontally from ground, and fills if it be grounded.
// 2. then find grounded voxels to fill and fill them.
struct HorizonSolver : public Solver {
  virtual ~HorizonSolver() override {}

  Trace solve(const Matrix& src, const Matrix& dst) final;
  const char* name() final { return "horizon"; }
};

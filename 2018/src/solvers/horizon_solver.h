#pragma once

#include <string>
#include "solver.h"

// HorizonSolver works as following.
// 1. scan horizontally from ground, and fills if it be grounded.
// 2. then find grounded voxels to fill and fill them.
class HorizonSolver : public Solver {
  Trace solve(const Matrix& src, const Matrix& dst) final;
  const char* name() final { return "horizon"; }
};

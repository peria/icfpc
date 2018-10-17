#pragma once

#include <string>

#include "solver.h"

// ClusterSolver fills voxels layer by layer using clusting.
struct ClusterSolver : public Solver {
  virtual ~ClusterSolver() override {}

  Trace solve(const Matrix& src, const Matrix& dst) final;
  const char* name() final { return "cluster"; }
};

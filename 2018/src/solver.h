#pragma once

#include "matrix.h"
#include "trace.h"

struct Solver {
  static Solver* GetSolver(const std::string& name);

  Trace solve(const Matrix& src, const Matrix& target);
};

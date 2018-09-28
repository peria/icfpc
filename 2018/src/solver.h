#pragma once

#include "matrix.h"
#include "trace.h"

struct Solver {
  static Solver* GetSolver(const std::string& name);

  virtual ~Solver() {}

  virtual Trace solve(const Matrix& src, const Matrix& dst) = 0;
  virtual const char* name() = 0;
};

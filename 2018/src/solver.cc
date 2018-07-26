#include "solver.h"

#include "solvers/simple_solver.h"

// static
Solver* Solver::GetSolver(const std::string& /*name*/) {
  return new SimpleSolver();
}

Trace Solver::solve(const Matrix& src, const Matrix& target) {
  // solve for test.
  return Trace();
}

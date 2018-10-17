#include "solver.h"

#include "solvers/simple_solver.h"
#include "solvers/horizon_solver.h"

// static
Solver* Solver::GetSolver(const std::string& /*name*/) {
  return new HorizonSolver();
}

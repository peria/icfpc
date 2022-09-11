#include "solver.h"

// static
std::shared_ptr<Solver> SolverRegistry::getSolver(const std::string& name) {
  return registry()[name].factory();
}

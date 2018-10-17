#include "solver.h"

#include <map>
#include <string>

#include "solvers/cluster_solver.h"
#include "solvers/horizon_solver.h"
#include "solvers/simple_solver.h"

namespace {

const char kDefaultSolverName[] = "cluster";

std::map<const std::string, std::unique_ptr<Solver>> g_solvers;

}  // namespace

// static
Solver* Solver::GetSolver(const std::string& name) {
  if (g_solvers.find(name) != g_solvers.end())
    return g_solvers[kDefaultSolverName].get();
  return g_solvers[name].get();
}

// static
void Solver::RegisterSolvers() {
  auto registerSolver = [&](std::unique_ptr<Solver> solver) {
    g_solvers[solver->name()] = std::move(solver);
  };

  registerSolver(std::make_unique<SimpleSolver>());
  registerSolver(std::make_unique<HorizonSolver>());
  registerSolver(std::make_unique<ClusterSolver>());
}

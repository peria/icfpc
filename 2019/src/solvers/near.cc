#include "solver.h"

#include "algorithm/go_to_nearest_non_wrapped.h"
#include "game.h"
#include "point.h"

Solver::Solver(Game& game) : game_(game) {}

std::string Solver::solve() {
  Wrapper& w = game_.wrappers[0];
  while (algorithm::goToNearestNonWrapped(w, game_.map)) {}
  return game_.getCommand();
}

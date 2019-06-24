#include "solver.h"

#include "game.h"

Solver::Solver(const std::string& desc_file, const std::string& buy_file)
  : game(desc_file, buy_file) {
}

std::string Solver::solve() {
  return "";
}

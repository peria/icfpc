#include "solver.h"

#include <iostream>

#include "game.h"

Solver::Solver(const std::string& desc, const std::string& buy)
    : game(desc, buy) {
}

std::string Solver::solve() {
  return "";
}

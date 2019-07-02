#include "solver.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "game.h"
#include "parser.h"

Solver::Solver(Game& game) : game_(game) {}

std::string Solver::solve() {
  return game_.getCommand();
}

#pragma once

#include <string>

#include "game.h"

class Solver {
 public:
  Solver(const std::string& desc, const std::string& buy = "");

  std::string solve();

  Game game;
};

#pragma once

#include <string>

#include "game.h"

class Solver {
 public:
  Solver(const std::string& desc_file, const std::string& buy_file);

  std::string solve();

  Game game;
};

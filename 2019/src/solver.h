#pragma once

#include <string>

#include "game.h"

class Solver {
 public:
  Solver(Game& game);

  std::string solve();

 private:
  Game& game_;
};

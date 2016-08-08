#pragma once

#include <string>

#include "core/game.h"

class Ai {
 public:
  Ai(const Game&, int punter_id);

  std::string name_;
  const int punter_id_;
  const Game& game_;
};

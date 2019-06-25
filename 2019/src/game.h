#pragma once

#include <array>
#include <string>

#include "booster.h"
#include "map.h"

class Game {
 public:
  Game(const std::string& desc, const std::string& buy = "");

  Map map;
  std::array<int, static_cast<int>(Booster::NumPortable)> num_boosters;
  // wrapper
  // boosters
  // time
};

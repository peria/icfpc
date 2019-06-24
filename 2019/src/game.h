#pragma once

#include <string>

#include "map.h"

class Game {
public:
  Game(const std::string& desc_file, const std::string& buy_file="");

  Map map;
  // wrapper
  // boosters
  // time
};

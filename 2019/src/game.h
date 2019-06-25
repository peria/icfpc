#pragma once

#include <string>

#include "map.h"

class Game {
public:
  Game(const std::string& desc, const std::string& buy="");

  Map map;
  // wrapper
  // boosters
  // time
};

#pragma once

#include <array>
#include <string>
#include <ostream>

#include "booster.h"
#include "map.h"
#include "wrapper.h"

class Game {
 public:
  Game(const std::string& desc, const std::string& buy = "");

  Map map;
  std::vector<Wrapper> wrappers;
  std::array<int, static_cast<int>(Booster::NumPortable)> num_boosters;
  int time = 0;
};

std::ostream& operator<<(std::ostream&, const Game&);


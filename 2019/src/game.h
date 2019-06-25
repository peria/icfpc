#pragma once

#include <array>
#include <ostream>
#include <string>
#include <unordered_map>

#include "booster.h"
#include "map.h"
#include "wrapper.h"

class Game {
  using Boosters = std::array<int, static_cast<int>(Booster::NumPortable)>;

 public:
  Game(const std::string& desc, const std::string& buy = "");

  Map map;
  std::vector<Wrapper> wrappers;
  Boosters num_boosters;
  int time = 0;
};

std::ostream& operator<<(std::ostream&, const Game&);


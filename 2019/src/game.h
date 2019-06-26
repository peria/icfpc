#pragma once

#include <array>
#include <ostream>
#include <string>
#include <unordered_map>

#include "booster.h"
#include "map.h"
#include "wrapper.h"

struct GameInitializer {
  GameInitializer(const std::string& desc, const std::string& buy = "");

  Map map;
  Point wrapper_initial_pos;
  std::array<int, static_cast<int>(Booster::NumPortable)> num_boosters;
};

class Game {
  using Boosters = std::array<int, static_cast<int>(Booster::NumPortable)>;

 public:
  Game(const GameInitializer& initializer);

  void pickUpBooster(const Point& pos);

  // Runs every wrappers in time line. (=valid mode)
  // TODO: Implement
  void replayFromInit(GameInitializer&);

  Map map;
  std::vector<Wrapper> wrappers;
  Boosters num_boosters;
  int time = 0;
};

std::ostream& operator<<(std::ostream&, const Game&);

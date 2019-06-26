#pragma once

#include <array>
#include <ostream>
#include <string>
#include <unordered_map>

#include "booster.h"
#include "map.h"
#include "wrapper.h"

// TODO: Construct Game with this class.
class GameInitializer;

class Game {
  using Boosters = std::array<int, static_cast<int>(Booster::NumPortable)>;

 public:
  Game(const std::string& desc, const std::string& buy = "");

  void pickUpBooster(const Point& pos);

  // Runs every wrappers in time line. (=valid mode)
  // TODO: Implement
  void runFromInit(GameInitializer&);

  Map map;
  std::vector<Wrapper> wrappers;
  Boosters num_boosters;
  int time = 0;
};

std::ostream& operator<<(std::ostream&, const Game&);

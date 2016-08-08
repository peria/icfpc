#pragma once

#include <string>
#include <vector>

#include "core/json.h"
#include "core/map.h"
#include "core/punter.h"
#include "core/settings.h"

class Game {
 public:
  Game(const int num_punters,
       const Map& map,
       const Settings& settings);
  void move(const Json& json);
  Json getScores() const;

  int num_punters() const { return punters_.size(); }
  int num_rivers() const { return map_.rivers.size(); }
  Punter& punter(int id) { return punters_[id]; }
  int playing_punter() const { return playing_punter_; }
  const Map& map() { return map_; }

 private:
  int playing_punter_ = 0;
  const Map& map_;
  const Settings& settings_;
  std::vector<Punter> punters_;
};

#include "core/game.h"

#include "core/json.h"

Game::Game(const int num_punters,
           const Map& map,
           const Settings& settings)
    : map_(map),
      settings_(settings) {
  for (int i = 0; i < num_punters; ++i) {
    punters_.emplace_back(i);
  }
}

void Game::move(const Json& json) {
  // TODO: impelement
  if (json.find("pass") != json.end()) {
  } else if (json.find("claim") != json.end()) {

  }

  playing_punter_ = (playing_punter_ + 1) % num_punters();
}

Json Game::getScores() const {
  Json message;
  for (const Punter& punter : punters_) {
    int id = punter.punter_id();
    int score = punter.score();
    message.push_back({
        {"punter", id},
        {"score", score}
      });
  }
  return message;
}

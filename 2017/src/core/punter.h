#pragma once

#include <string>

class Punter {
 public:
  enum class State {
    Setup,
    Gameplay,
    Scoring,
    Zombie,
  };
  enum class Move {
    Claim,
    Pass,
  };

  Punter(int id);
  void Pass() { ++num_timeout_; }

  void set_name(const std::string& name) { name_ = name; }
  int punter_id() const { return id_; }
  int score() const { return score_; }
  State state() const { return state_; }
  bool isZombie() const { return state_ == State::Zombie; }

 private:
  const int id_ = -1;
  std::string name_;
  State state_ = State::Setup;
  int score_ = 0;
  // 4.4 (Online|Offline) Being time-out for 10 times, they become a zombie punter.
  int num_timeout_ = 0;
};

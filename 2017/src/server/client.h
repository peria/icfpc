#pragma once

#include <string>
#include <vector>

#include "core/game.h"
#include "core/json.h"
#include "core/punter.h"

class Client {
 public:
  Client(const char* path, Punter& punter, Game& game);
  Json communicate();
  void reportScores(const Json& scores);

  Json messagePass(const Json& state=Json()) const;
  int getMessageSize() const;

  Punter& punter() const { return punter_; }
  bool isZombie() const { return punter().isZombie(); }

 private:
  bool sendMessage(const Json& message);
  Json recvMessage();

  // returns true if it succeeds.
  bool launch();
  bool shakeHands();
  bool setTimeout(int time_sec);
  bool terminate();

  const char* exe_path_ = nullptr;
  Punter& punter_;
  Game& game_;

  pid_t pid_;
  int read_fd_ = -1;
  int write_fd_ = -1;

  // Stacked moves to report in "moves".
  Json moves_;
};

#pragma once

#include <vector>

#include "matrix.h"
#include "nanobot.h"
#include "trace.h"

struct State {
  enum class Harmonics { kHigh, kLow };

  bool isWellFormed() const;

  // Returns total energy to run a trace.
  int execute();
  int step(Nanobot& bot, Command* command);

  int64 energy = 0;
  Harmonics harmonics;
  Matrix matrix;
  std::vector<Nanobot> bots;
  Trace trace;

  // Running state
  int time;
  int num_active_bots;
};

std::ostream& operator<<(std::ostream& ost, const State::Harmonics h) {
  switch (h) {
    case State::Harmonics::kHigh:
      ost << "High";
      break;
    case State::Harmonics::kLow:
      ost << "Low";
      break;
  }
  return ost;
}

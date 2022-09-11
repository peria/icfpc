#pragma once

#include "solver.h"

class EmptySolver final : public Solver {
 public:
  EmptySolver() : Solver("EmptySolver") {}
  Solution Solve(const Args& args) override;
};

REGISTER_SOLVER("EmptySolver", EmptySolver);

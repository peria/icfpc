#pragma once

#include <vector>

#include "action.h"
#include "memory.h"
#include "problem.h"

class Solver {
 public:
  std::vector<std::shared_ptr<Action>> Solve(std::shared_ptr<Problem> problem);
};
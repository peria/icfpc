#pragma once

#include <memory>
#include <vector>

#include "action.h"
#include "problem.h"

class Solver {
 public:
  std::vector<std::shared_ptr<Action>> Solve(std::shared_ptr<Problem> problem);
};
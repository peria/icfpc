#pragma once

#include <memory>
#include <vector>

#include "action.h"
#include "problem.h"
#include "solution.h"

class Solver {
 public:
  Solution Solve(std::shared_ptr<Problem> problem);
};
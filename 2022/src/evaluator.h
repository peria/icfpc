#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "action.h"
#include "problem.h"

class Evaluator {
 public:
  struct Result {
    std::int64_t action_cost = 0;
    std::int64_t similarity_cost = 0;
    std::int64_t cost = 0;
    std::int64_t num_actions = 0;
  };

  static Result evaluate(std::shared_ptr<Problem> problem,
                         const std::vector<std::shared_ptr<Action>>& actions);
};
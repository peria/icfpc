#pragma once

#include <memory>

#include "problem.h"
#include "solution.h"

class Solver {
 public:
  Solver(const Problem& problem) : problem_(problem) {}
  virtual ~Solver() = default;
  virtual std::unique_ptr<Solution> solve() = 0;

 protected:
  const Problem& problem_;
};

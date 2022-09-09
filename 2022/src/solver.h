#pragma once

#include <memory>
#include <string>

#include "action.h"
#include "problem.h"
#include "solution.h"

class Solver {
 public:
  struct Args {
    std::shared_ptr<Problem> problem;
  };

  Solver(const std::string& name) : name_(name) {}
  virtual ~Solver() = default;

  virtual Solution Solve(const Args& args) = 0;

  const std::string& name() { return name_; };

 private:
  const std::string name_;
};

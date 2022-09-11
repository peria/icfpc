#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "action.h"
#include "problem.h"
#include "solution.h"

class Solver {
 public:
  struct Args {
    std::shared_ptr<Problem> problem;
    // Actions that were made by other solvers.
    std::vector<std::shared_ptr<Action>> previous_actions;
  };

  Solver(const std::string& name) : name_(name) {}
  virtual ~Solver() = default;

  virtual Solution Solve(const Args& args) = 0;

  const std::string& name() { return name_; };

 private:
  const std::string name_;
};

struct SolverRegistry {
  struct SolverEntry {
    std::string file_name;
    std::function<std::shared_ptr<Solver>(void)> factory;
  };

  static std::map<std::string, SolverEntry>& registry() {
    static std::map<std::string, SolverEntry> s_registry;
    return s_registry;
  }

  static std::shared_ptr<Solver> getSolver(const std::string& name);

  SolverRegistry(const std::string& name, SolverEntry entry) {
    registry()[name] = entry;
  }
};

#define CONCAT_SUB(a, b) a##b
#define CONCAT(a, b) CONCAT_SUB(a, b)
#define REGISTER_SOLVER(name, cls)                              \
  static SolverRegistry CONCAT(_register_solver_, __LINE__) = { \
      name, {__FILE__, [] { return std::make_shared<cls>(); }}}

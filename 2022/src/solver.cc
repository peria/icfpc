#include "solver.h"

#include <cassert>
#include <memory>
#include <vector>

#include <glog/logging.h>

#include "action.h"
#include "problem.h"
#include "solution.h"

Solution Solver::Solve(std::shared_ptr<Problem> problem) {
  auto&& blocks = problem->canvas()->blocks();
  assert(blocks.size() > 0);
  auto&& root = blocks[0];

  std::vector<std::shared_ptr<Action>> actions;
  auto act = std::make_shared<HorizontalCutAction>(root->id(), 200);
  assert(act);
  actions.push_back(std::make_shared<HorizontalCutAction>(root->id(), 200));
  actions.push_back(
      std::make_shared<ColorAction>("0.0", Color(255, 0, 0, 255)));

  Solution solution;
  solution.actions = actions;
  return solution;
}

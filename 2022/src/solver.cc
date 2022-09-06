#include "solver.h"

#include <cassert>

#include <glog/logging.h>

#include "action.h"
#include "problem.h"

std::vector<std::shared_ptr<Action>> Solver::Solve(
    std::shared_ptr<Problem> problem) {
  auto&& blocks = problem->canvas()->blocks();
  assert(blocks.size() > 0);
  auto&& root = blocks[0];
  LOG(INFO) << "Block is " << root->id();

  std::vector<std::shared_ptr<Action>> actions;
  LOG(INFO) << "Before";
  auto act = std::make_shared<HorizontalCutAction>(root->id(), 200);
  LOG(INFO) << "Act is defined";
  assert(act);
  LOG(INFO) << "Action 1: " << act->toString();
  actions.push_back(std::make_shared<HorizontalCutAction>(root->id(), 200));
  LOG(INFO) << "ofl";
  actions.push_back(
      std::make_shared<ColorAction>("0.0", Color(255, 0, 0, 255)));
  LOG(INFO) << "ofl";
  return actions;
}

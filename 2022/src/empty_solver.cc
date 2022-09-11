#include "empty_solver.h"

#include "solution.h"

Solution EmptySolver::Solve(const Args& args) {
  auto canvas = args.problem->canvas();
  auto blocks = canvas->blocks();
  const auto& root_block_id = blocks[0]->id();
  std::shared_ptr<Action> action =
      std::make_shared<ColorAction>(root_block_id, Color(12, 34, 56, 78));

  Solution solution;
  solution.actions.push_back(action);
  return solution;
}

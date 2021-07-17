#pragma once

#include <memory>
#include <vector>

#include "solution.h"
#include "solver.h"

class ZeroDislikes final : public Solver {
 public:
  ZeroDislikes(const Problem& problem) : Solver(problem) {}
  ~ZeroDislikes() override = default;

  std::unique_ptr<Solution> solve() override;

 private:
  bool assign(int32 hole_vi,
              int32 pose_vi,
              std::vector<int32>& assigned_vertices,
              std::vector<bool>& used_vertices);
};
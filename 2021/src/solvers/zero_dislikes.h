#pragma once

#include <memory>
#include <vector>

#include "solution.h"
#include "solver.h"

class ZeroDislikes final : public Solver {
 public:
  ZeroDislikes(const Problem& problem);
  ~ZeroDislikes() override = default;

  std::unique_ptr<Solution> solve() override;

 private:
  bool assignOnHole(int32 hole_vi,
                    int32 pose_vi,
                    std::vector<bool>& used_vertices);
  bool isValidAssignment(int32 hole_vi,
                         int32 pose_vi,
                         const std::vector<bool>& used_vertices);

  std::vector<std::vector<double>> longests_;
  std::unique_ptr<Solution> solution_;
};
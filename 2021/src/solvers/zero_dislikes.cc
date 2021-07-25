#include "solvers/zero_dislikes.h"

#include <glog/logging.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "base.h"

ZeroDislikes::ZeroDislikes(const Problem& problem)
    : Solver(problem), solution_(new Solution(problem)) {
  const auto& vertices = problem.vertices;
  const int32 nv = vertices.size();
  const double rate = (1000000 + problem.epsilon) * 1e-6;

  longests_.resize(nv, std::vector<double>(nv, 1e+30));
  for (int32 i = 0; i < nv; ++i) {
    longests_[i][i] = 0;
    for (auto j : problem.vertices_next_to[i]) {
      if (j < i)
        continue;
      double d = std::sqrt(GetD2(vertices[i], vertices[j]));
      double max_d = d * rate * 1.00001;
      longests_[i][j] = max_d;
      longests_[j][i] = max_d;
    }
  }
  for (int32 k = 0; k < nv; ++k) {
    for (int32 i = 0; i < nv; ++i) {
      for (int32 j = 0; j < nv; ++j) {
        longests_[i][j] =
            std::min(longests_[i][j], longests_[k][i] + longests_[k][j]);
      }
    }
  }
}

std::unique_ptr<Solution> ZeroDislikes::solve() {
  const int32 num_vertices = problem_.vertices.size();
  std::vector<bool> used_vertices(num_vertices, false);
  for (int i = 0; i < num_vertices; ++i) {
    if (!assignOnHole(0, i, used_vertices)) {
      continue;
    }
    // Assignment of hole vertices are done.
    // TODO: Place other vertices.
    return std::move(solution_);
  }
  // Failed to assign
  std::cerr << "Failed to assign vertices.\n";
  return {};
}

bool ZeroDislikes::assignOnHole(int32 hole_vi,
                                int32 pose_vi,
                                std::vector<bool>& used_vertices) {
  const auto& hole = problem_.hole;
  if (hole_vi == hole.size())
    return true;

  used_vertices[pose_vi] = true;
  solution_->vertices[pose_vi] = hole[hole_vi];
  if (!isValidAssignment(hole_vi, pose_vi, used_vertices)) {
    used_vertices[pose_vi] = false;
    return false;
  }

  auto&& connected_pis = problem_.vertices_next_to[pose_vi];
  // Check edge-connected vertices first.
  for (int32 i : connected_pis) {
    if (used_vertices[i])
      continue;
    if (assignOnHole(hole_vi + 1, i, used_vertices))
      return true;
  }

  // Check non edge-connected vertices.
  const auto& vertices = problem_.vertices;
  for (int32 i = 0; i < vertices.size(); ++i) {
    if (used_vertices[i])
      continue;
    if (connected_pis.count(i) > 0)
      continue;

    bool is_reachable = true;
    for (int32 j = 0; j < vertices.size(); ++j) {
      if (!used_vertices[j] || i == j)
        continue;
      double d = longests_[i][j];
      if (GetD2(solution_->vertices[i], solution_->vertices[j]) > d * d) {
        is_reachable = false;
        break;
      }
    }
    if (!is_reachable)
      continue;

    if (assignOnHole(hole_vi + 1, i, used_vertices))
      return true;
  }

  used_vertices[pose_vi] = false;
  return false;
}

bool ZeroDislikes::isValidAssignment(int32 hole_vi,
                                     int32 pose_vi,
                                     const std::vector<bool>& used_vertices) {
  const auto& hole = problem_.hole;
  const auto& vertices = problem_.vertices;
  for (int32 pi : problem_.vertices_next_to[pose_vi]) {
    if (!used_vertices[pi])
      continue;
    Integer d2_0 = GetD2(vertices[pose_vi], vertices[pi]);
    const Segment edge(solution_->vertices[pose_vi], solution_->vertices[pi]);
    Integer d2 = GetD2(edge);
    if (!problem_.isAllowedD2(d2_0, d2))
      return false;
    for (int32 i = 0; i < hole.size(); ++i) {
      const Segment hole_edge{hole[i], hole[(i + 1) % hole.size()]};
      if (IsCrossed(hole_edge, edge))
        return false;
    }
    // TODO: Check if the edge is inside of the hole.
  }
  return true;
}
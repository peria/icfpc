#include "solvers/zero_dislikes.h"

#include <glog/logging.h>
#include <iostream>
#include <vector>

#include "base.h"

std::unique_ptr<Solution> ZeroDislikes::solve() {
  std::unique_ptr<Solution> solution(new Solution(problem_.vertices));

  std::vector<int32> assigned_vertices(problem_.hole.size(), -1);
  std::vector<bool> used_vertices(problem_.vertices.size(), false);
  for (int i = 0; i < problem_.vertices.size(); ++i) {
    if (!assign(0, i, assigned_vertices, used_vertices))
      continue;
    // Assignment of hole vertices are done.
    // TODO: Place other vertices.
    for (int32 i = 0; i < problem_.hole.size(); ++i) {
      int j = assigned_vertices[i];
      solution->vertices[j] = problem_.hole[i];
    }
    return std::move(solution);
  }
  // Failed to assign
  return {};
}

bool ZeroDislikes::assign(int32 hole_vi,
                          int32 pose_vi,
                          std::vector<int32>& assigned_vertices,
                          std::vector<bool>& used_vertices) {
  const auto& hole = problem_.hole;
  const auto& vertices = problem_.vertices;
  const int32 next_hole_vi = (hole_vi + 1) % hole.size();
  Integer hole_edge_d2 = Problem::getD2(hole[hole_vi], hole[next_hole_vi]);
  assigned_vertices[hole_vi] = pose_vi;
  used_vertices[pose_vi] = true;

  // Check the last edge of the hole.
  if (next_hole_vi == 0) {
    Integer edge_d2 =
        Problem::getD2(vertices[pose_vi], vertices[assigned_vertices[0]]);
    return problem_.isAllowedD2(edge_d2, hole_edge_d2);
  }

  // Check edge-connected vertices first.
  for (int32 i : problem_.vertices_next_to[pose_vi]) {
    if (used_vertices[i])
      continue;
    Integer edge_d2 = Problem::getD2(vertices[pose_vi], vertices[i]);
    if (!problem_.isAllowedD2(edge_d2, hole_edge_d2))
      continue;
    if (assign(next_hole_vi, i, assigned_vertices, used_vertices))
      return true;
  }
  // Check non edge-connected vertices.
  for (int32 i = 0; i < problem_.vertices.size(); ++i) {
  }

  used_vertices[pose_vi] = false;
  return false;
}
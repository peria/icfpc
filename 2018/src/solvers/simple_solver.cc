#include "simple_solver.h"

#include <set>
#include <vector>

#include "matrix.h"
#include "state.h"
#include "trace.h"

namespace {

using PII = std::pair<int, int>;

const Coordinate kNDs[] = {
    {1, 0, 0},  {-1, 0, 0},  {2, 0, 0},  {-2, 0, 0},  {0, 1, 0},  {0, -1, 0},
    {0, 2, 0},  {0, -2, 0},  {0, 0, 1},  {0, 0, -1},  {0, 0, 2},  {0, 0, -2},
    {1, 1, 0},  {-1, 1, 0},  {1, -1, 0}, {-1, -1, 0}, {1, 0, 1},  {-1, 0, 1},
    {1, 0, -1}, {-1, 0, -1}, {0, 1, 1},  {0, -1, 1},  {0, 1, -1}, {0, -1, -1},
};

// Direct diffs, which points to next voxels that each has a common face.
const Coordinate kDDs[] = {
    {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1},
};

std::vector<int> ComputeGroundDistance(const Matrix& mat) {
  const int R = mat.R;
  std::vector<int> dists(R * R * R, 1 << 30);  // Distance from ground
  std::set<PII> Q;
  for (int x = 0; x < R; ++x) {
    for (int y = 0; y < R; ++y) {
      if (mat(x, y, 0) != Voxel::kFull)
        continue;
      int i = (x * R + y) * R;
      dists[i] = 0;
      Q.insert(PII(0, i));
    }
  }

  while (!Q.empty()) {
    auto itr = Q.begin();
    int dist = itr->first + 1;
    int id = itr->second;
    Q.erase(itr);
    Coordinate c(id / R / R, id / R % R, id % R);
    for (const auto& dc : kDDs) {
      Coordinate cc = c + dc;
      if (!mat.isInRange(cc))
        continue;
      if (mat(cc) != Voxel::kFull)
        continue;
      int i = (cc.x * R + cc.y) * R + cc.z;
      if (dists[i] > dist) {
        dists[i] = dist;
        Q.insert(PII(dist, i));
      }
    }
  }

  return dists;
}

}  // namespace

Trace SimpleSolver::solve(const Matrix& src, const Matrix& dst) {
  std::vector<int> gdist = ComputeGroundDistance(dst);
  State state(src);

  /*
    while (dst.num_full != state.matrix.num_full)
      to_fill = {}
      while to_fill.size() < 20 || all(!v.is_ground for v in to_fill)
        to_fill.insert(close voxels)
      fill best evaled voxel
   */

  return std::move(state.trace);
};

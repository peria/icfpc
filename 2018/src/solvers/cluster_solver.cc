#include "cluster_solver.h"

#include <glog/logging.h>

#include <algorithm>
#include <chrono>
#include <vector>

#include "coordinate_union_find.h"
#include "matrix.h"
#include "state.h"
#include "trace.h"

namespace {

using PII = std::pair<int, int>;
using Clock = std::chrono::system_clock;

const ND kNDs[] = {
    {1, 0, 0},  {-1, 0, 0},  {0, 1, 0},  {0, -1, 0},  {0, 0, 1},  {0, 0, -1},
    {1, 1, 0},  {-1, 1, 0},  {1, -1, 0}, {-1, -1, 0}, {1, 0, 1},  {-1, 0, 1},
    {1, 0, -1}, {-1, 0, -1}, {0, 1, 1},  {0, -1, 1},  {0, 1, -1}, {0, -1, -1},
};

const ND kDDs[] = {
    {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1},
};

}  // namespace

Trace ClusterSolver::solve(const Matrix& src, const Matrix& dst) {
  const int R = src.R;
  State state(src);
  Nanobot& bot = state.bots[0];

  CoordinateUnionFind clusters;
  for (int x = 0; x < R; ++x) {
    for (int z = 0; z < R; ++z) {
      if (dst(x, 0, z) != Voxel::kFull)
        continue;
      Coordinate c(x, 0, z);
      clusters.Register(c);
    }
  }

  while (clusters.parent.size()) {
    clusters.Clustering();
    Coordinate near = clusters.GetClose(bot.position);
    CoordinateSet to_fills(clusters.GetCluster(near));
    auto computeToGo = [&](const Coordinate& from) {
      CoordinateMap<int> fillables;
      for (auto& c : to_fills) {
        CHECK_NE(state.matrix(c), Voxel::kFull) << c;
        for (auto& nd : kNDs) {
          Coordinate d(c + nd);
          if (!dst.isInRange(d) || state.matrix(d) != Voxel::kVoid)
            continue;
          fillables[d] = fillables[d] + 1;
        }
      }

      Coordinate ret(-1, -1, -1);  // invalid one
      int best_value = R * R * R * 100;
      for (const auto& fill : fillables) {
        Coordinate c(fill.first);
        int dist = (c - from).mLen();
        int value = dist * 50 + c.y;
        if (value < best_value) {
          ret = c;
          best_value = value;
        }
      }

      return ret;
    };

    // Exit if it takes longer than 8 secs.
    auto time_limit = Clock::now() + std::chrono::seconds(8);
    while (to_fills.size()) {
      if (Clock::now() >= time_limit) {
        LOG(INFO) << "Too long. abort. "
                  << "no ways found to fill " << to_fills.size() << " voxels.";
        return Trace();
      }

      // LOG(INFO) << bot.position << " " << to_fills.size();
      Coordinate to_go = computeToGo(bot.position);
      if (!bot.goTo(state.matrix, to_go)) {
        LOG(INFO) << "Failed to find a path from " << bot.position << " to "
                  << to_go << ".";
        return Trace();
      }

      CoordinateSet filleds;
      for (const ND& nd : kNDs) {
        Coordinate c(bot.position + nd);
        if (to_fills.count(c)) {
          bot.trace.emplace_back(std::make_unique<Fill>(nd));
          state.matrix(c) = Voxel::kFull;
          to_fills.erase(c);
          clusters.Unregister(c);
          filleds.insert(c);
        }
      }
      for (const Coordinate& filled : filleds) {
        for (const ND& dd : kDDs) {
          Coordinate c(filled + dd);
          if (dst.isInRange(c) && dst(c) == Voxel::kFull &&
              state.matrix(c) == Voxel::kVoid) {
            clusters.Register(c);
          }
        }
      }
    }
  }

  if (!bot.goTo(state.matrix, Coordinate(0, 0, 0))) {
    LOG(INFO) << "Failed to go back home from " << bot.position;
    return Trace();
  }

  state.collectTrace();
  state.execute();
  return std::move(state.trace);
};

#include "cluster_solver.h"

#include <glog/logging.h>

#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
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

  CoordinateUnionFind to_fill_clusters;
  for (int x = 0; x < R; ++x) {
    for (int z = 0; z < R; ++z) {
      if (dst(x, 0, z) != Voxel::kFull)
        continue;
      Coordinate c(x, 0, z);
      to_fill_clusters.Register(c);
    }
  }

  while (true) {
    Coordinate cluster(-1, -1, -1);
    to_fill_clusters.Clustering();
    {
      int close_distance = R * 10;
      for (auto& p : to_fill_clusters.parent) {
        int d = (bot.position - p.first).mLen();
        if (d < close_distance) {
          close_distance = d;
          cluster = p.first;
        }
      }
    }
    cluster = to_fill_clusters.Find(cluster);

    std::unordered_set<Coordinate, Coordinate::Hash> to_fills;
    for (auto& p : to_fill_clusters.parent) {
      if (to_fill_clusters.Find(p.first) == cluster)
        to_fills.insert(p.first);
    }

    auto computeToGo = [&](const Coordinate& from) {
      std::unordered_map<Coordinate, int, Coordinate::Hash> fillables;
      for (auto& c : to_fills) {
        for (auto& nd : kNDs) {
          Coordinate d(c + nd);
          if (!src.isInRange(d) || state.matrix(d) != Voxel::kVoid)
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
    do {
      std::unordered_set<Coordinate, Coordinate::Hash> next_to_fills;
      while (to_fills.size()) {
        if (Clock::now() >= time_limit) {
          LOG(INFO) << "Too long. abort. "
                    << "no ways found to fill " << to_fills.size() << " voxels.";
          return Trace();
        }

        Coordinate to_go = computeToGo(bot.position);
        if (!bot.goTo(state.matrix, to_go)) {
          LOG(INFO) << "Failed to find a path from " << bot.position
                    << " to " << to_go << ".";
          return Trace();
        }

        std::vector<Coordinate> filleds;
        for (int dy = 0; dy <= 1; ++dy) {
          if (filleds.size())
            break;
          for (const ND& nd : kNDs) {
            if (nd.y > dy)
              continue;
            Coordinate c(bot.position + nd);
            auto itr = to_fills.find(c);
            if (itr != to_fills.end()) {
              bot.trace.emplace_back(std::make_unique<Fill>(nd));
              state.matrix(c) = Voxel::kFull;
              to_fills.erase(itr);
              next_to_fills.erase(c);
              filleds.push_back(c);
            }
          }
        }
        for (const Coordinate& filled : filleds) {
          for (const ND& dd : kDDs) {
            Coordinate c(filled + dd);
            if (dst.isInRange(c) && dst(c) == Voxel::kFull &&
                state.matrix(c) == Voxel::kVoid) {
              next_to_fills.insert(c);
            }
          }
        }
      }
      to_fills = next_to_fills;
    } while (to_fills.size());
  }

  if (!bot.goTo(state.matrix, Coordinate(0, 0, 0))) {
    LOG(INFO) << "Failed to go back home from " << bot.position;
    return Trace();
  }

  state.collectTrace();
  state.execute();
  return std::move(state.trace);
};

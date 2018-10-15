#include "simple_solver.h"

#include <glog/logging.h>

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "matrix.h"
#include "state.h"
#include "trace.h"

namespace {

using PII = std::pair<int, int>;

const ND kNDs[] = {
    {1, 0, 0},  {-1, 0, 0},  {0, 1, 0},  {0, -1, 0},  {0, 0, 1},  {0, 0, -1},
    {1, 1, 0},  {-1, 1, 0},  {1, -1, 0}, {-1, -1, 0}, {1, 0, 1},  {-1, 0, 1},
    {1, 0, -1}, {-1, 0, -1}, {0, 1, 1},  {0, -1, 1},  {0, 1, -1}, {0, -1, -1},
};

const ND kDDs[] = {
    {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1},
};

}  // namespace

Trace SimpleSolver::solve(const Matrix& src, const Matrix& dst) {
  const int R = src.R;
  State state(src);

  if (R > 20) {
    LOG(INFO) << "No work for R=" << R;
    return Trace();
  }

  std::unordered_set<Coordinate, Coordinate::Hash> to_fills;
  for (int x = 0; x < R; ++x) {
    for (int z = 0; z < R; ++z) {
      if (dst(x, 0, z) != Voxel::kFull)
        continue;
      to_fills.emplace(x, 0, z);
    }
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

  Nanobot& bot = state.bots[0];
  int stacking = 0;
  Coordinate stack_pos(-1, -1, -1);
  size_t stack_size = 0;
  do {
    std::unordered_set<Coordinate, Coordinate::Hash> next_to_fills;
    while (to_fills.size()) {
      if (stack_size != to_fills.size() || stack_pos != bot.position)
        stacking = -1;
      if (++stacking > 10) {
        LOG(INFO) << "too long. abort. "
                  << "no ways found to fill " << to_fills.size() << " voxels.";
        to_fills.clear();
        next_to_fills.clear();
        break;
      }
      stack_size = to_fills.size();
      stack_pos = bot.position;
      // LOG(INFO) << bot.position << " " << to_fills.size();

      bot.goTo(state.matrix, computeToGo(bot.position));

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

  bot.goTo(state.matrix, Coordinate(0, 0, 0));

  state.collectTrace();
  state.execute();
  return std::move(state.trace);
};

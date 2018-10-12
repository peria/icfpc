#include "simple_solver.h"

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
      int value = dist * 20 - fill.second;
      if (value < best_value) {
        ret = c;
        best_value = value;
      }
    }

    return ret;
  };

  Nanobot& bot = state.bots[0];
  while (to_fills.size()) {
    Coordinate to_go(computeToGo(bot.position));
    {
      Trace route(state.matrix.findPath(bot.position, to_go));
      for (auto& cmd : route) {
        state.trace.push_back(std::move(cmd));
      }
    }
    bot.position = to_go;

    for (const ND& nd : kNDs) {
      Coordinate c(to_go + nd);
      auto itr = to_fills.find(c);
      if (itr != to_fills.end()) {
        state.trace.emplace_back(std::make_unique<Fill>(nd));
        state.matrix(c) = Voxel::kFull;
        to_fills.erase(itr);
        for (const ND& dd : kDDs) {
          Coordinate g(c + dd);
          if (dst.isInRange(g) && dst(g) == Voxel::kFull &&
              state.matrix(g) == Voxel::kVoid) {
            to_fills.insert(g);
          }
        }
      }
    }
  }

  {
    Trace route(state.matrix.findPath(bot.position, Coordinate(0, 0, 0)));
    for (auto& cmd : route) {
      state.trace.push_back(std::move(cmd));
    }
  }

  state.trace.emplace_back(std::make_unique<Halt>());
  LOG(INFO) << "commands: " << state.trace.size();
  LOG(INFO) << "energy: " << state.energy;
  return std::move(state.trace);
};

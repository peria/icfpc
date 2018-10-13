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
        if (nd.y < 0)
          continue;

        Coordinate d(c + nd);
        if (!src.isInRange(d) || state.matrix(d) != Voxel::kVoid)
          continue;
        fillables[d] = fillables[d] + 1 + nd.y;
      }
    }

    Coordinate ret(-1, -1, -1);  // invalid one
    int best_value = R * R * R * 100;
    for (const auto& fill : fillables) {
      Coordinate c(fill.first);
      int dist = (c - from).mLen();
      int value = dist * 50 + c.y * 10 - fill.second;
      if (value < best_value) {
        ret = c;
        best_value = value;
      }
    }

    return ret;
  };

  Nanobot& bot = state.bots[0];
  int no_move_count = 0;
  while (to_fills.size()) {
    LOG(INFO) << bot.position << " " << to_fills.size();
    Coordinate to_go(computeToGo(bot.position));
    if (!dst.isInRange(to_go)) {
      LOG(INFO) << "nowhere to go";
      break;
    }

    {
      Trace route(state.matrix.findPath(bot.position, to_go));
      if (route.size() == 0) {
        if (++no_move_count > 18) {
          LOG(INFO) << "no move";
          break;
        }
      } else {
        no_move_count = 0;
      }
      for (auto& cmd : route) {
        state.trace.push_back(std::move(cmd));
      }
    }
    bot.position = to_go;

    std::vector<Coordinate> filleds;
    for (const ND& nd : kNDs) {
      if (nd.y > 0)
        continue;
      Coordinate c(to_go + nd);
      auto itr = to_fills.find(c);
      if (itr != to_fills.end()) {
        state.trace.emplace_back(std::make_unique<Fill>(nd));
        state.matrix(c) = Voxel::kFull;
        to_fills.erase(itr);
        filleds.push_back(c);
      }
    }
    for (const Coordinate& c : filleds) {
      for (const ND& dd : kDDs) {
        Coordinate g(c + dd);
        if (dst.isInRange(g) && dst(g) == Voxel::kFull &&
            state.matrix(g) == Voxel::kVoid) {
          to_fills.insert(g);
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

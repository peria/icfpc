#include "matrix.h"

#include <algorithm>
#include <deque>
#include <fstream>
#include <set>
#include <unordered_map>
#include <vector>

Matrix::Matrix(const std::string& filename) : R(0) {
  if (filename.empty()) {
    // No specified.
    return;
  }

  std::ifstream ifs(filename, std::ifstream::binary);
  if (!ifs) {
    std::cerr << "Fail to load " << filename << ".\n";
    return;
  }

  ifs.seekg(0, ifs.end);
  int size = ifs.tellg();
  ifs.seekg(0, ifs.beg);
  std::vector<char> buffer(size);
  ifs.read(buffer.data(), size);
  ifs.close();

  R = static_cast<uint8>(buffer[0]);
  voxels_.resize(R * R * R, Voxel::kVoid);
  for (int x = 0; x < R; ++x) {
    for (int y = 0; y < R; ++y) {
      for (int z = 0; z < R; ++z) {
        int index = (x * R + y) * R + z;
        int byte = index / 8 + 1;
        int bit = index % 8;
        if ((buffer[byte] >> bit) & 1) {
          voxels_[index] = Voxel::kFull;
        }
      }
    }
  }
}

bool Matrix::isGrounded(int /*x*/, int /*y*/, int /*z*/) const {
  NOT_IMPLEMENTED();
  return false;
}

bool Matrix::isAllGrounded() const {
  NOT_IMPLEMENTED();
  return false;
}

namespace {

struct Step {
  int step;
  std::unique_ptr<Command> command;
};

struct Compare {
  Compare(const Coordinate& to,
          const std::unordered_map<Coordinate, Step, Coordinate::Hash>& steps)
      : to(to), steps(steps) {}

  bool operator()(const Coordinate& a, const Coordinate& b) {
    auto ia = steps.find(a);
    auto ib = steps.find(b);
    if (ia == steps.end())
      return false;
    if (ib == steps.end())
      return true;

    const Step& sa = ia->second;
    const Step& sb = ib->second;
    int va = sa.step + (to - a).mLen();
    int vb = sb.step + (to - b).mLen();
    if (va != vb)
      return va < vb;
    return sa.step > sb.step;
  }

  const Coordinate& to;
  const std::unordered_map<Coordinate, Step, Coordinate::Hash>& steps;
};

}  // namespace

Trace Matrix::findPath(const Coordinate& from, const Coordinate& to) const {
  DCHECK_EQ(Voxel::kVoid, operator()(to)) << to;

  // Look for a trace to reach |to| from |from| through void voxels
  // using A* algorithm.
  std::unordered_map<Coordinate, Step, Coordinate::Hash> steps;
  steps[from] = Step{0, nullptr};
  std::deque<Coordinate> Q;
  Compare cmp(to, steps);
  Q.push_back(from);
  while (Q.size() && steps.find(to) == steps.end()) {
    std::sort(Q.begin(), Q.end(), cmp);
    Coordinate c = Q.front();
    Q.pop_front();

    int step = steps[c].step + 1;
    auto check = [&](const LLD& d) {
      Coordinate t = c + d;
      if (!isInRange(t) || operator()(t) != Voxel::kVoid)
        return false;
      auto itr = steps.find(t);
      if (itr != steps.end() && itr->second.step <= step)
        return false;

      Q.push_back(t);
      steps[t] = Step{step, std::make_unique<SMove>(d)};
      return true;
    };
    for (int d = 1; d <= 15 && check(LLD{d, 0, 0}); ++d) {
    }
    for (int d = 1; d <= 15 && check(LLD{0, d, 0}); ++d) {
    }
    for (int d = 1; d <= 15 && check(LLD{0, 0, d}); ++d) {
    }
    for (int d = 1; d <= 15 && check(LLD{-d, 0, 0}); ++d) {
    }
    for (int d = 1; d <= 15 && check(LLD{0, -d, 0}); ++d) {
    }
    for (int d = 1; d <= 15 && check(LLD{0, 0, -d}); ++d) {
    }
  }

  Trace trace;
  Coordinate c(to);
  while (steps[c].command) {
    trace.push_back(std::move(steps[c].command));
    c -= trace.back()->To<SMove>()->lld;
  }
  std::reverse(trace.begin(), trace.end());
  return std::move(trace);
}

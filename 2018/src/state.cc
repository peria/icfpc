#include "state.h"

#include <unordered_set>

#include "coordinate.h"

bool State::isWellFormed() const {
  if (harmonics != Harmonics::kLow)
    return false;
  if (!matrix.isAllGrounded())
    return false;

  std::unordered_set<int> actives;
  for (auto& bot : bots) {
    const Coordinate& c = bot.position;
    if (!matrix.isInRange(c))
      return false;
    // Check dup
    if (!actives.insert(c.hash()).second)
      return false;
  }
  return true;
}

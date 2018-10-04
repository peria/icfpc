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

int State::execute() {
  time = 0;
  energy = 0;
  num_active_bots = 1;

  auto cmd = trace.begin();
  while (true) {
    // run a time step
    const int n = num_active_bots;
    const int coef = (harmonics == Harmonics::kHigh) ? 30 : 3;
    energy += coef * matrix.R * matrix.R * matrix.R;
    energy += 20 * n;

    if ((*cmd)->type() == Command::kHalt) {
      step(bots[0], cmd->get());
      ++cmd;
      break;
    }

    for (int i = 0; i < n; ++i) {
      energy += step(bots[i], cmd->get());
      ++cmd;
    }
    ++time;
  }
  DCHECK(cmd == trace.end());

  return energy;
}

int State::step(Nanobot& bot, Command* command) {
  switch (command->type()) {
    case Command::kHalt: {
      CHECK_EQ(0, bot.position.hash());
      CHECK_EQ(1u, num_active_bots);
      CHECK_EQ(Harmonics::kLow, harmonics);
      return 0;
    }
    case Command::kWait: {
      return 0;
    }
    case Command::kFlip: {
      harmonics =
          (harmonics == Harmonics::kHigh) ? Harmonics::kLow : Harmonics::kHigh;
      return 0;
    }
    case Command::kSMove: {
      const LLD& lld = command->toSMove()->lld;
      bot.position += lld;
      return 2 * lld.mLen();
    }
    case Command::kLMove: {
      const SLD& sld1 = command->toLMove()->sld1;
      const SLD& sld2 = command->toLMove()->sld2;

      bot.position += sld1;
      bot.position += sld2;
      return 2 * (sld1.mLen() + 2 + sld2.mLen());
    }
    case Command::kFission: {
      const ND& nd = command->toFission()->nd;
      const int m = command->toFission()->m;

      Nanobot& nbot = bots[num_active_bots++];
      nbot.position = bot.position;
      nbot.position += nd;
      NOT_IMPLEMENTED();
      return 24;
    }
    case Command::kFill: {
      NOT_IMPLEMENTED();
      return 0;
    }
    case Command::kVoid: {
      NOT_IMPLEMENTED();
      return 0;
    }
    case Command::kFusionP: {
      NOT_IMPLEMENTED();
      return 0;
    }
    case Command::kFusionS: {
      NOT_IMPLEMENTED();
      return -24;
    }
    case Command::kGFill: {
      NOT_IMPLEMENTED();
      return 0;
    }
    case Command::kGVoid: {
      NOT_IMPLEMENTED();
      return 0;
    }
    case Command::kSync: {
      LOG(INFO) << "running Sync command";
      return 0;
    }
  }

  NOT_IMPLEMENTED();
  return 0;
}

#include "state.h"

#include <algorithm>
#include <unordered_set>
#include <vector>

#include "coordinate.h"

namespace {

const int kNumNanobots = 20;  // Lightning
// const int kNumNanobots = 40;  // Full contest

}  // namespace

State::State(const Matrix& matrix) : matrix(matrix), bots(kNumNanobots) {
  // Initialize bots
  for (int i = 0; i < kNumNanobots; ++i)
    bots[i].bid = i + 1;
  bots[0].seeds = (1ull << kNumNanobots) - 2;
  bots[0].is_active = true;
}

bool State::isWellFormed() const {
  if (harmonics != Harmonics::kLow)
    return false;
  if (!matrix.isAllGrounded())
    return false;

  std::unordered_set<Coordinate, Coordinate::Hash> actives;
  for (auto& bot : bots) {
    const Coordinate& c = bot.position;
    if (!matrix.isInRange(c))
      return false;
    // Check dup
    if (!actives.insert(c).second)
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
    Command* command = cmd->get();
    const int n = num_active_bots;
    const int coef = (harmonics == Harmonics::kHigh) ? 30 : 3;
    energy += coef * matrix.R * matrix.R * matrix.R;
    energy += 20 * n;

    std::vector<Collaboration> collaborations;
    if (command->type() == Command::kHalt) {
      step(bots[0], command, collaborations);
      ++cmd;
      break;
    }

    bool need_sort = false;
    for (int i = 0; i < n; ++i) {
      energy += step(bots[i], command, collaborations);
      need_sort = (need_sort || command->needSort());
      ++cmd;
    }

    // Process collaborations
    ProcessFusion(collaborations);

    // Re-sort bots.
    if (need_sort) {
      std::sort(bots.begin(), bots.end(),
                [](const Nanobot& a, const Nanobot& b) {
                  if (a.is_active == b.is_active)
                    return a.bid < b.bid;
                  return a.is_active;
                });
    }
    ++time;
  }
  DCHECK(cmd == trace.end());

  return energy;
}

int State::step(Nanobot& bot,
                Command* command,
                std::vector<Collaboration>& collaborations) {
  switch (command->type()) {
    case Command::kHalt: {
      CHECK_EQ(Coordinate(0, 0, 0), bot.position);
      CHECK_EQ(1u, num_active_bots);
      CHECK_EQ(Harmonics::kLow, harmonics);
      CHECK_EQ(1, bot.bid);
      bot.is_active = false;
      --num_active_bots;

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
      DCHECK(bot.seeds);

      int bid = bot.takeSeed1();
      Nanobot& nbot = findBot(bid);
      DCHECK(!nbot.is_active);
      nbot.is_active = true;
      nbot.position = bot.position + nd;
      nbot.seeds = bot.takeSeeds(m);

      return 24;
    }
    case Command::kFill: {
      const ND& nd = command->toFill()->nd;

      Coordinate c = bot.position + nd;
      if (matrix(c) == Voxel::kVoid) {
        matrix(c) = Voxel::kFull;
        return 12;
      }
      DCHECK_EQ(Voxel::kFull, matrix(c));
      return 0;
    }
    case Command::kVoid: {
      const ND& nd = command->toVoid()->nd;

      Coordinate c = bot.position + nd;
      if (matrix(c) == Voxel::kFull) {
        matrix(c) = Voxel::kVoid;
        return -12;
      }
      DCHECK_EQ(Voxel::kVoid, matrix(c));
      return 3;
    }
    case Command::kFusionP: {
      collaborations.emplace_back(Collaboration{bot, command});
      return 0;
    }
    case Command::kFusionS: {
      collaborations.emplace_back(Collaboration{bot, command});
      return -24;
    }
    case Command::kGFill: {
      collaborations.emplace_back(Collaboration{bot, command});
      return 0;
    }
    case Command::kGVoid: {
      collaborations.emplace_back(Collaboration{bot, command});
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

Nanobot& State::findBot(int bid) {
  return *find_if(bots.begin(), bots.end(),
                  [bid](const Nanobot& b) { return bid == b.bid; });
}

void State::ProcessFusion(std::vector<Collaboration>& collaborations) {
  for (auto collab : collaborations) {
    if (collab.command->type() != Command::kFusionP)
      continue;
    Coordinate dst = collab.bot.position + collab.command->toFusionP()->nd;
    auto sec = find_if(collaborations.begin(), collaborations.end(),
                       [dst](const Collaboration& col) {
                         return col.command->type() == Command::kFusionS &&
                                col.bot.position == dst;
                       });
    if (sec == collaborations.end()) {
      LOG(ERROR) << "Cannot find opposite bot to fusion with bot["
                 << collab.bot.bid << "]";
      exit(1);
    }
    DCHECK_EQ(collab.bot.position,
              sec->bot.position + sec->command->toFusionS()->nd);
    collab.bot.fuse(sec->bot);
    --num_active_bots;
  }
}

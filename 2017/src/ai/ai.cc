#include "ai.h"

Ai::Ai(const Game& game, int punter_id)
    : name_("punter"),
      punter_id_(punter_id),
      game_(game) {}

#include "punter.h"

Punter::Punter(int id)
    : id_(id),
      state_(State::Setup) {}

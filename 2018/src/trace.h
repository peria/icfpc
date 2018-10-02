#pragma once

#include <deque>
#include <memory>
#include <string>

#include "command.h"

class Trace : public std::deque<std::unique_ptr<Command>> {
 public:
  std::string toString() const;
  void dump(const std::string& filename) const;
};

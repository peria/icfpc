#pragma once

#include <deque>
#include <memory>
#include <string>
#include <vector>

#include "command.h"

class Trace : public std::deque<std::unique_ptr<Command>> {
 public:
  std::vector<uint8> toString() const;
  void dump(const std::string& filename) const;
};

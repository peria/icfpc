#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "action.h"

struct Solution {
  void Save(const std::filesystem::path& file_path);

  std::vector<std::shared_ptr<Action>> actions;
};

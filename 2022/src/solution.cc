#include "solution.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <glog/logging.h>

void Solution::Save(const std::filesystem::path& file_path) {
  std::ofstream ofs(file_path);
  if (ofs.fail()) {
    LOG(ERROR) << "Failed to save file: " << file_path;
    std::cerr << "Filed to save file " << file_path << "\n"
              << "The solution is dumped to standard error.\n\n";
    for (auto&& action : actions) {
      std::cerr << action->toString() << "\n";
    }
    return;
  }

  for (auto&& action : actions) {
    ofs << action->toString() << "\n";
  }
  LOG(INFO) << "Solution is saved.";
}

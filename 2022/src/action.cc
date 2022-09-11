#include "action.h"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include <glog/logging.h>

namespace {
std::shared_ptr<Action> LineToAction(const std::string& line) {
  if (line.size() > 290) {
    LOG(ERROR) << "Too long ISL line \"" << line << "\"";
    return nullptr;
  }

  {
    char block_id[300];
    char dir;
    int val;
    if (std::sscanf(line.c_str(), "cut [%[^]]] [%c] [%d]", block_id, &dir,
                    &val) == 3) {
      if (std::toupper(dir) == 'X') {
        return std::make_shared<VerticalCutAction>(block_id, val);
      } else if (std::toupper(dir) == 'Y') {
        return std::make_shared<HorizontalCutAction>(block_id, val);
      }
      LOG(WARNING) << "Failed to parse: \"" << line << "\"";
      return nullptr;
    }
  }
  {
    char block_id[300];
    int val1, val2;
    if (std::sscanf(line.c_str(), "cut [%[^]]] [%d,%d]", block_id, &val1,
                    &val2) == 3) {
      return std::make_shared<PointCutAction>(block_id, val1, val2);
    }
  }
  {
    char block_id[300];
    int r, g, b, a;
    if (std::sscanf(line.c_str(), "color [%[^]]] [%d,%d,%d,%d]", block_id, &r,
                    &g, &b, &a) == 5) {
      return std::make_shared<ColorAction>(block_id, Color(r, g, b, a));
    }
  }
  {
    char block_id0[150], block_id1[150];
    if (std::sscanf(line.c_str(), "merge [%[^]]] [%[^]]]", block_id0,
                    block_id1) == 2) {
      return std::make_shared<MergeAction>(block_id0, block_id1);
    }
  }
  {
    char block_id0[150], block_id1[150];
    if (std::sscanf(line.c_str(), "swap [%[^]]] [%[^]]]", block_id0,
                    block_id1) == 2) {
      return std::make_shared<SwapAction>(block_id0, block_id1);
    }
  }
  if (line.size() > 0 && line[0] == '#') {
    return std::make_shared<CommentAction>(line.substr(1));
  }
  LOG(WARNING) << "Failed to parse: \"" << line << "\"";

  return nullptr;
}
}  // namespace

std::vector<std::shared_ptr<Action>> Action::LoadIsl(
    const std::filesystem::path& file_path) {
  std::vector<std::shared_ptr<Action>> actions;
  std::ifstream ifs(file_path);
  for (std::string line; std::getline(ifs, line);) {
    auto action = LineToAction(line);
    actions.push_back(action);
  }
  return actions;
}

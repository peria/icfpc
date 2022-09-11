#include "problem.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <glog/logging.h>
#include <nlohmann/json.hpp>

#include "block.h"
#include "canvas.h"
#include "image.h"

using Json = nlohmann::json;

std::shared_ptr<Problem> Problem::Load(const std::filesystem::path& filepath) {
  // Target painting must be loaded successfully.
  Json target_json;
  {
    auto target_path = filepath;
    target_path.replace_extension("json");
    std::ifstream ifs(target_path);
    if (ifs.fail()) {
      LOG(ERROR) << "Fail to load " << target_path;
      return nullptr;
    }
    ifs >> target_json;
  }

  int width = Canvas::kDefaultWidth;
  int height = Canvas::kDefaultHeight;
  std::vector<std::shared_ptr<Block>> blocks;
  {
    auto initial_path = filepath;
    initial_path.replace_extension("initial.json");
    Json initial_json;
    std::ifstream ifs(initial_path);
    if (!ifs.fail()) {
      LOG(INFO) << "Load initial file: " << initial_path;
      ifs >> initial_json;
      width = initial_json["width"];
      height = initial_json["height"];
      // TODO: Load blocks.
    }
  }

  std::vector<Color> source(width * height);
  {
    auto source_path = filepath;
    source_path.replace_extension("source.json");
    Json source_json;
    std::ifstream ifs(source_path);
    if (!ifs.fail()) {
      LOG(INFO) << "Load source file: " << source_path;
      ifs >> source_json;
      for (int i = 0; i < width * height; ++i) {
        auto& c = source_json[i];
        source[i] = Color(c[0], c[1], c[2], c[3]);
      }
    }
  }

  std::vector<Color> target_colors(width * height);
  for (int i = 0; i < width * height; ++i) {
    auto& c = target_json[i];
    target_colors[i] = Color(c[0], c[1], c[2], c[3]);
  }

  auto canvas = std::make_shared<Canvas>(width, height, source, blocks);
  auto target = std::make_shared<Image>(width, height, target_colors);
  return std::make_shared<Problem>(canvas, target);
}

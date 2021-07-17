#include "solution.h"

#include <nlohmann/json.hpp>
#include <fstream>

void Solution::saveFile(const char* filepath) {
  using Json = nlohmann::json;
  Json jvertices;
  for (auto&& vertex : vertices) {
    jvertices.push_back({vertex.x, vertex.y});
  }

  Json solution;
  solution["vertices"] = jvertices;
  std::ofstream ofs(filepath);
  ofs << solution;
}

#include "solution.h"

#include <fstream>
#include <nlohmann/json.hpp>

Solution::Solution(const Problem& p) : vertices(p.vertices), problem(p) {}

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

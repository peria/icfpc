#include "problem.h"

#include <fstream>
#include <iostream>
#include <string>

#include "bonus.h"
#include "nlohmann/json.hpp"

Problem::Problem(const char* filepath) {
  parseJsonFile(filepath);

  vertices_next_to.resize(vertices.size());
  for (auto&& edge : edges) {
    int32 i = edge.first;
    int32 j = edge.second;
    vertices_next_to[i].insert(j);
    vertices_next_to[j].insert(i);
  }
}

void Problem::parseJsonFile(const char* filepath) {
  using Json = nlohmann::json;

  std::ifstream ifs(filepath);
  Json j = Json::parse(std::string(std::istreambuf_iterator<char>(ifs),
                                   std::istreambuf_iterator<char>()));
  for (auto&& jvertex : j["hole"]) {
    Integer x = jvertex[0];
    Integer y = jvertex[1];
    hole.push_back({x, y});
  }
  auto figure = j["figure"];
  for (auto&& jvertex : figure["vertices"]) {
    Integer x = jvertex[0];
    Integer y = jvertex[1];
    vertices.push_back({x, y});
  }
  for (auto&& jedge : figure["edges"]) {
    int i = jedge[0];
    int j = jedge[1];
    edges.push_back({i, j});
  }
  epsilon = j["epsilon"];
  for (auto&& jbonus : j["bonuses"]) {
    auto&& jposition = jbonus["position"];
    Bonus bonus;
    bonus.position = Point{jposition[0], jposition[1]};
    bonus.type = Bonus::GetType(jbonus["bonus"]);
    bonus.problem = jbonus["problem"];
    bonuses.push_back(bonus);
  }
}
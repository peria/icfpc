#include "problem.h"

#include <fstream>
#include <iostream>

Problem::Problem(const std::string& str) {
  std::istringstream iss(str);
  init(iss);
}

Problem::Problem(std::istream& is) {
  init(is);
}

bool Problem::loadFile(const std::string& filename) {
  std::ifstream ifs(filename);
  if (!ifs.good()) {
    std::cerr << "Failed to load " << filename << "\n";
    return false;
  }
  init(ifs);
  return true;
}

bool Problem::init(std::istream& is) {
  int num_polygons;
  if (!(is >> num_polygons))
    return false;
  for (int i = 0; i < num_polygons; ++i)
    silhouettes_.emplace_back(is);

  int num_lines;
  if (!(is >> num_lines))
    return false;
  for (int i = 0; i < num_lines; ++i)
    skeltons_.emplace_back(is);

  return true;
}

std::ostream& operator<<(std::ostream& os, const Problem& problem) {
  os << "Silhoutette (" << problem.silhouettes().size() << ")\n"
     << "Skelton (" << problem.skeltons().size() << ")\n";
  return os;
}

#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "polygon.h"

class Problem {
 public:
  Problem() = default;
  Problem(const std::string& str);
  Problem(std::istream& is);
  ~Problem() = default;

  bool loadFile(const std::string& filename);
  bool init(std::istream& is);

  const std::vector<Polygon>& silhouettes() const { return silhouettes_; }
  const std::vector<Line>& skeltons() const { return skeltons_; }

 private:
  std::vector<Polygon> silhouettes_;
  std::vector<Line> skeltons_;
};

std::ostream& operator<<(std::ostream&, const Problem&);


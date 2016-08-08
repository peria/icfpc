#include <iostream>
#include <glog/logging.h>
#include "base.h"
#include "solution.h"
using namespace std;

Fraction Solution::area() const {
  return width() * height();
}

Fraction Solution::width() const {
  Fraction xmin = destinations[0].x;
  Fraction xmax = destinations[0].x;
  for (auto& v : destinations) {
    xmin = min(xmin, v.x);
    xmax = max(xmax, v.x);
  }
  return xmax - xmin;
}

Fraction Solution::height() const {
  Fraction ymin = destinations[0].y;
  Fraction ymax = destinations[0].y;
  for (auto& v : destinations) {
    ymin = min(ymin, v.y);
    ymax = max(ymax, v.y);
  }
  return ymax - ymin;
}

ostream& operator<<(ostream& os, const Solution& solution) {
  os << solution.sources.size() << "\n";
  for (const Vertex& src : solution.sources) {
    os << src << "\n";
  }

  os << solution.facets.size() << "\n";
  for (const vector<int>& facet : solution.facets) {
    os << facet.size();
    for (int v : facet) {
      os << " " << v;
    }
    os << "\n";
  }

  for (const Vertex& dest : solution.destinations) {
    os << dest << "\n";
  }

  return os;
}

#include "polygon.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "vertex.h"
using namespace std;

Vertex Polygon::edge(int id) const {
  const Vertex& from = vertexs[id % vertexs.size()];
  const Vertex& to = vertexs[(id + 1) % vertexs.size()];
  return {to.x - from.x, to.y - from.y};
}

Fraction Polygon::width() const {
  Fraction xmin = vertexs[0].x;
  Fraction xmax = vertexs[0].x;
  for (auto& v : vertexs) {
    xmin = min(xmin, v.x);
    xmax = max(xmax, v.x);
  }
  return xmax - xmin;
}

Fraction Polygon::height() const {
  Fraction ymin = vertexs[0].y;
  Fraction ymax = vertexs[0].y;
  for (auto& v : vertexs) {
    ymin = min(ymin, v.y);
    ymax = max(ymax, v.y);
  }
  return ymax - ymin;
}

istream& operator>>(istream& is, Polygon& polygon) {
  int numVertex;
  is >> numVertex;
  for (int i = 0; i < numVertex; ++i) {
    Vertex vertex;
    is >> vertex;
    polygon.vertexs.push_back(vertex);
  }
  return is;
}

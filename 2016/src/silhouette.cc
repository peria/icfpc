#include "silhouette.h"

#include <iostream>
using namespace std;

bool Silhouette::isAsIs() {
  if (polygons.size() != 1)
    return false;

  const Polygon& polygon = polygons[0];
  if (polygon.size() != 4)
    return false;

  for (int i = 0; i < 4; ++i) {
    if (polygon.edge(i).length2() != 1) {
      return false;
    }
  }

  for (int i = 0; i < 4; ++i) {
    if (dotProd(polygon.edge(i), polygon.edge(i + 1)) != 0) {
      return false;
    }
  }

  return true;
}

istream& operator>>(istream& is, Silhouette& silhouette) {
  int numPolygon;
  is >> numPolygon;
  for (int i = 0; i < numPolygon; ++i) {
    Polygon polygon;
    is >> polygon;
    silhouette.polygons.push_back(polygon);
  }
  return is;
}

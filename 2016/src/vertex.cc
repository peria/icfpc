#include "vertex.h"

#include <iostream>
#include <string>
#include <sstream>

#include <gmpxx.h>
#include <glog/logging.h>

#include "base.h"
using namespace std;

Fraction Vertex::length2() const {
  return dotProd(*this, *this);
}

ostream& operator<<(ostream& os, const Vertex& vertex) {
  os << vertex.x.get_num();
  if (vertex.x.get_den() != 1)
    os << "/" << vertex.x.get_den();
  os << "," << vertex.y.get_num();
  if (vertex.y.get_den() != 1)
    os << "/" << vertex.y.get_den();
  return os;
}

istream& operator>>(istream& is, Vertex& vertex) {
  char comma;
  is >> vertex.x >> comma >> vertex.y;
  DCHECK_EQ(',', comma);
  return is;
}

Fraction dotProd(const Vertex& a, const Vertex& b) {
  return a.x * b.x + a.y * b.y;
}

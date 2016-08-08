#pragma once

#include "base.h"

struct Vertex {
  Fraction length2() const;

  Fraction x;
  Fraction y;
};

std::ostream& operator<<(std::ostream& os, const Vertex& vertex);
std::istream& operator>>(std::istream& is, Vertex& vertex);

Fraction dotProd(const Vertex& a, const Vertex& b);

#pragma once

#include <iostream>
#include <vector>

#include "base.h"
#include "silhouette.h"
#include "skeleton.h"
#include "solution.h"

struct Problem {
  const Solution solve();

  Silhouette silhouette;
  Skeleton skeleton;

 private:
  const Solution solveAsIs();
  const Solution solveWrap();

  const Solution solveWrapCore(const Polygon& polygon);
  std::vector<Fraction> getPoints(const std::vector<Vertex>&, Fraction&, int index);
};

std::istream& operator>>(std::istream& is, Problem& problem);

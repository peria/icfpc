#include "image.h"

#include <memory>
#include <vector>

#include <glog/logging.h>

// static
double Image::ComputeSimilarity(std::shared_ptr<Image> a,
                                std::shared_ptr<Image> b) {
  static constexpr double kAlpha = 0.005;

  auto&& sa = a->getSerialized();
  auto&& sb = b->getSerialized();
  CHECK_EQ(sa.size(), sb.size());

  const int n = sa.size();
  double diff = 0;
  for (int i = 0; i < n; ++i) {
    diff += Color::distance(sa[i], sb[i]);
  }
  return diff * kAlpha;
}

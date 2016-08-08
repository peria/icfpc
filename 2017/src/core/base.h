#pragma once

#include <cstdint>
#include <cctype>

#define CHECK(c)                                             \
  do {                                                       \
    if (!(c)) {                                              \
      std::cerr << "CHECK failure: "                         \
                << #c << " returns false." << std::endl;     \
    }                                                        \
  } while (false)

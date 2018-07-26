#include "trace.h"

#include <iostream>
#include <string>
#include <fstream>

void Trace::dump(const std::string& filename) const {
  std::ofstream ofs(filename, std::ofstream::binary);
  if (!ofs) {
    std::cerr << "Fail to output to " << filename << "\n";
    return;
  }
}

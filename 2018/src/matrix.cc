#include "matrix.h"

#include <fstream>
#include <vector>

Matrix::Matrix(const std::string& filename)
  : R(0) {
  if (filename.empty()) {
    // No specified.
    return;
  }

  std::ifstream ifs(filename, std::ifstream::binary);
  if (!ifs) {
    std::cerr << "Fail to load " << filename << ".\n";
    return;
  }

  ifs.seekg(0, ifs.end);
  int size = ifs.tellg();
  ifs.seekg(0, ifs.beg);
  std::vector<char> buffer(size);
  ifs.read(buffer.data(), size);
  ifs.close();

  R = static_cast<int>(buffer[0]);
  voxels_.resize(R * R * R, Voxel::kEmpty);
  for (int x = 0; x < R; ++x) {
    for (int y = 0; y < R; ++y) {
      for (int z = 0; z < R; ++z) {
        int index = (x * R + y) * R + z;
        int byte = index / 8 + 1;
        int bit = index % 8;
        if ((buffer[byte] >> bit) & 1) {
          voxels_[index] = Voxel::kFull;
        }
      }
    }
  }
}

bool Matrix::isGrounded(const Coordinate& c) const {
  NOT_IMPLEMENTED();
  return false;
}

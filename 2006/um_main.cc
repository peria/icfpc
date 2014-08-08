#include "um.h"

#include <fstream>
#include <iostream>

namespace {
const Platter kMask1 = 0x00FF00FFU;
const Platter kMask2 = 0x0000FFFFU;

Platter Endian(Platter operation) {
  operation = ((operation >>  8) & kMask1) | ((operation & kMask1) <<  8);
  operation = ((operation >> 16) & kMask2) | ((operation & kMask2) << 16);
  return operation;
}
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <UM file>\n";
    return 0;
  }

  std::ifstream ifs;
  ifs.open(argv[1], std::ios::binary);
  if (!ifs.is_open()) {
    std::cerr << "Usage: " << argv[0] << " <UM file>\n";
    return 0;
  }

  ifs.seekg(0, std::ios_base::end);
  int size = ifs.tellg();
  ifs.seekg(0, std::ios_base::beg);
  if (size == -1) {
    std::cerr << "Usage: " << argv[0] << " <UM file>\n";
    return 0;
  }

  size /= sizeof(Platter);
  std::cerr << "FileSize: " << size << " platters\n";
  Program program(new Platter[size + 1]);
  ifs.read(reinterpret_cast<char*>(program.get() + 1), size * sizeof(Platter));
  ifs.close();
  program[0] = size;
  
  // Endian change
  for (int i = 1; i <= size; ++i)
    program[i] = Endian(program[i]);

  UM um(std::move(program));
  um.Run();

  return 0;
}

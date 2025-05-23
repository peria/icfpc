#include "um.h"

#include <fstream>
#include <iostream>

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
  Memory program(size);
  ifs.read(reinterpret_cast<char*>(program.get()), size * sizeof(Platter));
  ifs.close();

  program.ConvertEndian();

  UM um(std::move(program));
  um.Run();

  return 0;
}

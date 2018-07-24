#pragma once

#include <cstdint>
#include <memory>
#include <map>
#include <vector>

typedef uint32_t Platter;
typedef std::unique_ptr<Platter[]> Array;
struct Memory {
  size_t size;
  Array array;
};

class UM {
 public:
  UM(Array program, size_t size);
  ~UM();

  void Run();
  bool Step(const Platter& operation);

 private:
  int pc_;  // Program counter;
  Platter registers_[8] {};  // General purpose
  std::vector<Memory> memory_;
  Platter memory_base_;
};

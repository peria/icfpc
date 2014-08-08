#pragma once

#include <cstdint>
#include <memory>
#include <map>
#include <vector>

typedef uint32_t Platter;
typedef std::unique_ptr<Platter[]> Program;

class UM {
 public:
  UM(Program program);
  ~UM();

  void Run();
  bool Step(const Platter& operation);

 private:
  int pc_;  // Program counter;
  Platter registers_[8] {};  // General purpose
  std::vector<Program> memory_;
  Platter memory_base_;
};

#pragma once

#include <cstdint>
#include <map>
#include <vector>

typedef uint32_t Platter;
typedef std::unique_ptr<Platter> Program;

class UM {
 public:
  UM(Program program);
  ~UM();

  void Run();
  void Step(const Platter& operation);

 private:
  Program zero_array_;
  Platter* program_;
  int pc_;  // Program counter;
  Platter registers_[8];  // General purpose
  std::map<Platter, std::unique_ptr<Platter> > memory_;
};

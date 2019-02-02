#pragma once

#include <cstdint>
#include <memory>
#include <map>
#include <vector>

using Platter = std::uint32_t;

struct Memory {
  Memory(std::unique_ptr<Platter[]>&& a, size_t s)
      : array(std::move(a)), size(s) {}
  Memory(size_t s) : array(std::make_unique<Platter[]>(s)), size(s) {}
  Platter* get() const { return array.get(); }

  void reset() {
    array.reset();
    size = 0;
  }
  void resize(size_t s) {
    array.reset(new Platter[s]);
    size = s;
  }

  void ConvertEndian();

  std::unique_ptr<Platter[]> array;
  size_t size;
};

class UM {
 public:
  UM(Memory&& program);
  ~UM() = default;

  void Run();
  bool Step(const Platter& operation);

  Memory& memory(Platter slot) {
    return memory_[slot];
  }

 private:
  int pc_;                  // Program counter;
  Platter registers_[8]{};  // General purpose
  std::vector<Memory> memory_;

  int num_free_ = 0;
  int num_alloc_ = 0;
};

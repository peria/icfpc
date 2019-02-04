#pragma once

#include <cstdint>
#include <memory>
#include <map>
#include <vector>

using Platter = std::uint32_t;

struct Memory {
  Memory(std::unique_ptr<Platter[]>&& a, size_t s)
      : array(std::move(a)), active(1), size(s) {}
  Memory(size_t s)
      : Memory(std::make_unique<Platter[]>(s), s) {}
  Platter* get() const { return array.get(); }

  void reset() {
    array.reset();
    active = 0;
    size = 0;
  }
  void resize(size_t s) {
    array.reset(new Platter[s]);
    active = 1;
    size = s;
  }

  void ConvertEndian();

  std::unique_ptr<Platter[]> array;
  size_t active: 1;
  size_t size: 31;
};

class UM {
  static constexpr int kMaxSlots = 20000000;
 public:
  UM(Memory&& program);
  ~UM() = default;

  void Run();
  bool Step(const Platter& operation);

  Memory& memory(Platter slot) {
    return memory_[slot];
  }

 private:
  int pc_ = 0;              // Program counter;
  Platter registers_[8]{};  // General purpose
  std::vector<Memory> memory_;
  int next_ = 1;

  int num_free_ = 0;
  int num_alloc_ = 0;
};

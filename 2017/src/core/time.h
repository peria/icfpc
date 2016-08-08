#pragma once

#include <chrono>

class Time {
  using Clock = std::chrono::high_resolution_clock;
 public:
  using TimeStamp = Clock::time_point;

  static TimeStamp now() { return Clock::now(); }
  static TimeStamp setTimeLimitFromNow(int time_ms);
};

#include "time.h"

Time::TimeStamp Time::setTimeLimitFromNow(int time_ms) {
  return now() + std::chrono::milliseconds(time_ms);
}

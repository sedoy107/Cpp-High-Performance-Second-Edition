#pragma once

#include <iostream>
#include <chrono>

#define USE_TIMER 1

#if USE_TIMER
#define MEASURE_FUNCTION()                                                     \
  ScopedTimer timer { __func__ }
#else
#define MEASURE_FUNCTION()
#endif

// ScopedTimer from Chapter 3
class ScopedTimer {

public:
    using ClockType = std::chrono::steady_clock;

  ScopedTimer(const char* func) :
    function_{func}, start_{ClockType::now()} {
  }

  ScopedTimer(const ScopedTimer&) = delete;
  ScopedTimer(ScopedTimer&&) = delete;
  auto operator=(const ScopedTimer&) -> ScopedTimer& = delete;
  auto operator=(ScopedTimer&&) -> ScopedTimer& = delete;

  ~ScopedTimer() {
    using namespace std::chrono;
    auto stop = ClockType::now();
    auto duration = (stop - start_);
    auto micro = duration_cast<microseconds>(duration).count();
    std::cout << micro << " micro sec " << function_ <<  '\n';
  }

private:
  const char* function_ = {};
  const ClockType::time_point start_ = {};
};

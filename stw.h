#ifndef CPPSTOPWATCH_STW_H_
#define CPPSTOPWATCH_STW_H_

#include <chrono>
#include <memory>
#include <stdexcept>

namespace stw {

using nano = std::chrono::nanoseconds;
using micro = std::chrono::microseconds;
using milli = std::chrono::milliseconds;
using sec = std::chrono::seconds;
using min = std::chrono::minutes;
using hour = std::chrono::hours;

class Stopwatch {
 public:
  using Rep = long long int;
  using Clock = std::chrono::high_resolution_clock;

  Stopwatch() noexcept : running_(false), elapsed_time_(0) {}

  virtual ~Stopwatch() {}
  
  Stopwatch* Start() {
    if (running_) {
      throw std::runtime_error("Stopwatch has already started");
    }

    start_time_ = Clock::now();
    running_ = true;
    return this;
  }

  Stopwatch* Stop() {
    if (!running_) {
      throw std::runtime_error("Stopwatch has not yet started");
    }

    elapsed_time_ += Clock::now() - start_time_;
    running_ = false;
    return this;
  }

  Stopwatch* Reset() noexcept {
    elapsed_time_ = decltype(elapsed_time_)::zero();
    running_ = false;
    return this;
  }

  template <class TimeUnit>
  Rep Elapsed() const noexcept {
    auto time = elapsed_time_;
    if (running_) {
      time += Clock::now() - start_time_;
    }
    return std::chrono::duration_cast<TimeUnit>(time).count();
  }

  bool IsRunning() const noexcept {
    return running_;
  }

 protected:
  bool running_;
  std::chrono::time_point<Clock> start_time_;
  std::chrono::duration<Rep, std::nano> elapsed_time_;
};

std::unique_ptr<Stopwatch> CreateStarted() noexcept {
  std::unique_ptr<Stopwatch> sw(new Stopwatch);
  sw->Start();
  return sw;
}

std::unique_ptr<Stopwatch> CreateUnstarted() noexcept {
  return std::unique_ptr<Stopwatch>(new Stopwatch);
}

} // namespace stw

namespace std {

template <class E, class T>
basic_ostream<E, T>& operator<<(basic_ostream<E, T> &os,
                                const stw::Stopwatch &sw) {
  auto time = sw.Elapsed<stw::nano>();
  if (time < 1000) {
    os << time << " ns";
  } else if (time < 1000 * 1000) {
    os << time / 1000.0 << " µs";
  } else if (time < 1000 * 1000 * 1000) {
    os << time / 1000.0 / 1000.0 << " ms";
  } else {
    os << time / 1000.0 / 1000.0 / 1000.0 << " s";
  }
  
  return os;
}

template <class E, class T>
basic_ostream<E, T>& operator<<(basic_ostream<E, T> &os,
                                const unique_ptr<stw::Stopwatch> &sw) {
  return os << *sw.get();
}

template <class E, class T>
basic_ostream<E, T>& operator<<(basic_ostream<E, T> &os,
                                const shared_ptr<stw::Stopwatch> &sw) {
  return os << *sw.get();
}

} // namespace std

#endif  // CPPSTOPWATCH_STW_H_
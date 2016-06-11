#ifndef CPPSTOPWATCH_STW_H_
#define CPPSTOPWATCH_STW_H_

#include <memory>

namespace stw {

class Stopwatch;

std::unique_ptr<Stopwatch> CreateStarted() {
  return nullptr;
}

std::unique_ptr<Stopwatch> CreateUnstarted() {
  return nullptr;
}

class Stopwatch {
 public:
  Stopwatch& Start() {
    return *this;
  }

  Stopwatch& Stop() {
    return *this;
  }

  Stopwatch& Reset() {
    return *this;
  }

  template <class TimeUnit>
  long Elapsed() {
    return 0;
  }

  bool IsRunning() {
    return true;
  }
};

} // namespace stw

namespace std {

template <class E, class T>
basic_ostream<E, T>& operator<<(basic_ostream<E, T>& os,
                                const stw::Stopwatch &sw);

template <class E, class T>
basic_ostream<E, T>& operator<<(basic_ostream<E, T>& os,
                                const shared_ptr<stw::Stopwatch> &sw);

} // namespace std

#endif  // CPPSTOPWATCH_STW_H_
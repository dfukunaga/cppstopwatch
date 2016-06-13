#include <cassert>
#include <iostream>
#include <sstream>
#include <thread>
#include "stw.h"

namespace {

template <class Except, class Func, class... Args>
void AssertThrow(Func&& f, Args&&... args) {
  try {
    f(std::forward<Args>(args)...);
    assert(false);
  } catch (Except &e) {
    // ok
  } catch (...) {
    assert(false);
  }
}

void StopwatchTest() {
  auto sw = std::make_shared<stw::Stopwatch>();
  std::cout << "- check initialization" << std::endl;
  assert(sw->Elapsed<std::chrono::nanoseconds>() == 0);
  assert(sw->IsRunning() == false);

  std::cout << "- check start" << std::endl;  
  sw->Start();
  assert(sw->IsRunning() == true);
  AssertThrow<std::runtime_error>([&sw](){sw->Start();});

  std::cout << "- check stop" << std::endl;    
  sw->Stop();
  assert(sw->IsRunning() == false);
  AssertThrow<std::runtime_error>([&sw](){sw->Stop();});

  std::cout << "- check elapsed" << std::endl;      
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  auto elapsed = sw->Elapsed<stw::nano>();
  assert(elapsed > 0);
  sw->Start();
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  assert(sw->Elapsed<stw::nano>() > elapsed);
  sw->Stop();

  std::cout << "- check reset" << std::endl;      
  sw->Reset();
  assert(sw->IsRunning() == false);
  assert(sw->Elapsed<stw::nano>() == 0);
  sw->Start()->Reset();
  assert(sw->IsRunning() == false);
  assert(sw->Elapsed<stw::nano>() == 0);

  std::cout << "- check started creation" << std::endl;      
  sw = stw::CreateStarted();
  AssertThrow<std::runtime_error>([&sw](){sw->Start();});
  sw->Stop();

  std::cout << "- check unstarted creation" << std::endl;      
  sw = stw::CreateUnstarted();
  AssertThrow<std::runtime_error>([&sw](){sw->Stop();});
  sw->Start();
  
  std::cout << "- check method chain" << std::endl;      
  sw->Reset()->Start()->Stop()->Elapsed<stw::milli>();

  std::cout << "- check measurement result" << std::endl;      
  sw->Reset()->Start();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  sw->Stop();
  elapsed = sw->Elapsed<stw::milli>();
  assert(elapsed >= 100.0 && elapsed < 100.0 * 1.1);

  std::cout << "- check stream output" << std::endl;      
  std::stringstream ss;
  sw->Reset()->Start();
  sw->Stop();
  ss << sw;
  assert(ss.str().substr(ss.str().length()-2) == "ns");
  sw->Reset()->Start();
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  sw->Stop();
  ss << sw;
  assert(ss.str().substr(ss.str().length()-3) == "µs"); // µ is 2 bytes?
  sw->Reset()->Start();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  sw->Stop();
  ss << sw;
  assert(ss.str().substr(ss.str().length()-2) == "ms");
  sw->Reset()->Start();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  sw->Stop();
  ss << sw;
  assert(ss.str().substr(ss.str().length()-1) == "s");
}

} // namespace

int main(int argc, char **argv) {
  ::StopwatchTest();
}
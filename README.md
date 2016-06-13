# cppstopwatch
Single file stopwatch library for C++

## Usage
1. Copy `stw.h` to your project.
2. Write code and build using C++11 compiler.

## Example
```c++
#include "stw.h"

{
  auto sw = stw::CreateStarted();
  doSomething();
  std::cout << sw->Stop() << std::endl;
}

{
  auto sw = stw::CreateUnstarted();
  
  sw->Start();
  doSomething1();
  sw->Stop();

  if (!sw->IsRunning) {
    sw->Start();
  }
  doSomething2();
  sw->Stop();

  std::cout << sw << std::endl;
}

{
  auto sw = stw::CreateUnstarted();

  sw->Start();
  doSomething1();
  auto elapsed1 = sw->Stop()->Elapsed<stw::milli>();

  sw->Reset()->Start();
  doSomething2();
  auto elapsed2 = sw->Stop()->Elapsed<stw::milli>();

  std::cout << "elapsed1 = " << elapsed1 << " ms"
            << "elapsed2 = " << elapsed2 << " ms" << std::endl;
}
```
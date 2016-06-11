#include <cassert>
#include "stw.h"

namespace {

void StopwatchTest() {
  auto sw = stw::CreateUnstarted();
}

} // namespace

int main(int argc, char **argv) {
  ::StopwatchTest();
}
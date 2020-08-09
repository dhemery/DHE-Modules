#pragma once
#include "components/Latch.h"
#include "helpers/fake-controls.h"
namespace test {
namespace fake {
  using dhe::Latch;

  class Interrupter {
  public:
    std::function<bool(int, Latch const &)> isInterrupted{
        [](int s, Latch const &l) -> bool { throw "Interrupter.isInterrupted(" + std::to_string(s) + ", latch)"; }};
  };
} // namespace fake
} // namespace test

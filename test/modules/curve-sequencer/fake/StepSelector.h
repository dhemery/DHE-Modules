#pragma once

#include <functional>

namespace test {
namespace curve_sequencer {
  namespace fake {
    struct StepSelector {
      std::function<int()> first{[]() -> int { throw "StepSelector.first(start)"; }};
      std::function<int(int, bool)> successor{
          [](int s, bool l) -> int { throw "StepSelector.successor(start,looping)"; }};
    };
  } // namespace fake
} // namespace curve_sequencer
} // namespace test

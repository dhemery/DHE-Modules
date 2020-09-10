#pragma once

#include <functional>

namespace test {
namespace curve_sequencer {
  struct FakeStepSelector {
    std::function<int()> first{[]() -> int { throw "FakeStepSelector.first(start)"; }};
    std::function<int(int, bool)> successor{
        [](int s, bool l) -> int { throw "FakeStepSelector.successor(start,looping)"; }};
  };
} // namespace curve_sequencer
} // namespace test

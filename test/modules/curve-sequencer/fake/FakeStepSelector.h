#pragma once

#include <functional>

namespace test {
namespace curve_sequencer {
  struct FakeStepSelector {
    std::function<int()> first;
    std::function<int(int, bool)> successor;
  };
} // namespace curve_sequencer
} // namespace test

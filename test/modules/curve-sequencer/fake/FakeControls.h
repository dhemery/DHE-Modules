#pragma once

#include <functional>

namespace test {

namespace curve_sequencer {
  struct FakeControls {
    std::function<float()> input;
    std::function<bool()> isGated;
    std::function<bool()> isLooping;
    std::function<bool()> isReset;
    std::function<bool()> isRunning;
    std::function<void(float)> output;
  };

} // namespace curve_sequencer
} // namespace test

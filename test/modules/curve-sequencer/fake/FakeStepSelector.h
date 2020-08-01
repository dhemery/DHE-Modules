#pragma once

#include <functional>

namespace curvesequencertest {
namespace fake {
  struct StepSelector {
    std::function<int()> firstFunc;
    std::function<int(int, bool)> successorFunc;

    auto first() const -> int { return firstFunc(); }
    auto successor(int step, bool looping) const -> int { return successorFunc(step, looping); }
  };
} // namespace fake
} // namespace curvesequencer

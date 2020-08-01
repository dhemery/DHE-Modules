#pragma once

#include <functional>

namespace curvesequencertest {
namespace fake {
  struct Controls {
    std::function<float()> inputFunc;
    std::function<bool()> isGatedFunc;
    std::function<bool()> isLoopingFunc;
    std::function<bool()> isResetFunc;
    std::function<bool()> isRunningFunc;
    std::function<void(float)> outputFunc;

    auto input() const -> float { return inputFunc(); }

    auto isGated() const -> bool { return isGatedFunc(); }

    auto isLooping() const -> bool { return isLoopingFunc(); }

    auto isReset() const -> bool { return isResetFunc(); }

    auto isRunning() const -> bool { return isRunningFunc(); }

    void output(float voltage) { outputFunc(voltage); }
  };

} // namespace fake
} // namespace curvesequencer
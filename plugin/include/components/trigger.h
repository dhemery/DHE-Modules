#include <utility>

#pragma once

#include "edge-detector.h"

namespace DHE {

/**
 * Tracks a boolean value and generates one or more events whenever the value rises.
 */
class Trigger : public EdgeDetector {
public:
  explicit Trigger(std::function<void()> on_rise)
      : on_rise{std::move(on_rise)} {}

private:
  /**
   * The event generated whenever the tracked value changes.
   * @param state the new value of the tracked value
   */
  void on_state_change(bool state) override {
    if (state) {
      on_rise();
    }
  }

private:
  const std::function<void()> on_rise;
};
} // namespace DHE

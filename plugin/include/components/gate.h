#pragma once

#include <utility>

#include "trigger.h"

namespace DHE {

/**
 * Tracks a boolean value and generates an event whenever the value rises or
 * falls.
 */
class Gate : public EdgeDetector {
public:
  Gate(std::function<void()> on_rise, std::function<void()> on_fall)
      : on_rise{std::move(on_rise)}, on_fall{std::move(on_fall)} {}

protected:
  void on_state_change(bool state) override {
    if (state) {
      on_rise();
    } else {
      on_fall();
    }
  }

private:
  const std::function<void()> on_rise;
  const std::function<void()> on_fall;
};
} // namespace DHE

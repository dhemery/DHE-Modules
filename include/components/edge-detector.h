#pragma once
#include <functional>
#include <utility>

namespace DHE {

/**
 * Tracks a boolean value and generates an event whenever the value changes.
 */
class EdgeDetector {
public:
  EdgeDetector(std::function<bool()> state_in, std::function<void()> on_rise,
               std::function<void()> on_fall)
      : state_in{std::move(state_in)}, on_rise{std::move(on_rise)},
        on_fall{std::move(on_fall)} {}

  /**
   * Retrieves the current state of the tracked value and generates an event
   * if the state changes.
   */
  void step() {
    if (state_in() == state)
      return;
    state = !state;
    if (state)
      on_rise();
    else
      on_fall();
  }

private:
  bool state = false;
  const std::function<bool()> state_in;
  const std::function<void()> on_rise;
  const std::function<void()> on_fall;
};
} // namespace DHE

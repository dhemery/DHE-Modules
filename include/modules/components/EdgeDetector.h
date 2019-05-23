#pragma once
#include <functional>
#include <utility>

namespace dhe {

/**
 * Tracks a boolean value and generates an event whenever the value changes.
 */
class EdgeDetector {
public:
  EdgeDetector(std::function<bool()> stateIn, std::function<void()> onRise,
               std::function<void()> onFall)
      : stateIn{std::move(stateIn)}, onRise{std::move(onRise)},
        onFall{std::move(onFall)} {}

  /**
   * Retrieves the current state of the tracked value and generates an event
   * if the state changes.
   */
  void step() {
    if (stateIn() == state) {
      return;
    }
    state = !state;
    if (state) {
      onRise();
    } else {
      onFall();
    }
  }

private:
  bool state = false;
  const std::function<bool()> stateIn;
  const std::function<void()> onRise;
  const std::function<void()> onFall;
};
} // namespace dhe

#pragma once

#include <functional>
#include <vector>

#include "latch.hpp"

namespace DHE {

/**
 * A flip-flop is a latch that determines its state based on an incoming boolean
 * signal.
 */
class DFlipFlop : public Latch {
public:
  /**
   * Creates a flip-flop that determines its state based on the given boolean
   * signal.
   *
   * @param signal supplies the boolean signal to evaluate
   */
  template <typename Signal>
  explicit DFlipFlop(const Signal &signal) : signal{signal} {}

  /**
   * Sets the state by comparing its current state to the signal.
   * - Fires 'rise' if the state changes to HIGH.
   * - Fires 'fall' if the state changes to LOW.
   */
  void step() {
    if (signal() && !is_high())
      set_state(State::HIGH);
    else if (!signal() && is_high())
      set_state(State::LOW);
  }

private:
  const std::function<bool()> signal;
};
} // namespace DHE

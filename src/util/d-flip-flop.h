#ifndef DHE_UTIL_D_FLIP_FLOP_H
#define DHE_UTIL_D_FLIP_FLOP_H

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "latch.h"

namespace DHE {

/**
 * A latch determines its state by comparing the incoming signal to the specified
 * thresholds.
 */
struct DFlipFlop : public Latch {

  /**
   * Creates a flip-flop that compares the given signal to the given thresholds.
   *
   * @param signal called on each step to obtain the value to evaluate
   * @param low_threshold signal value at or below which the flip-flop state is LOW
   * @param high_threshold signal value at or above which the flip-flop state is HIGH
   */
  DFlipFlop(std::function<float()> signal, float low_threshold, float high_threshold)
      : signal{std::move(signal)},
        low_threshold{low_threshold},
        high_threshold{high_threshold} {}

  /**
   * Creates a flip-flop that compares the given signal to a low threshold of 0
   * and a high threshold of 1.
   *
   * @param signal called on each step to obtain the signal to evaluate
   */
  explicit DFlipFlop(const std::function<float()> &signal)
      : DFlipFlop{signal, 0.0f, 1.0f} {}

  /**
   * Sets the state by comparing the signal to the thresholds.
   * - Fires rising-edge if the state changed to HIGH.
   * - Fires falling-edge if the state changed to LOW.
   * - Fires no-change if the state did not change.
   */
  void step() {
    if (signal() >= high_threshold)
      set_state(HIGH);
    else if (signal() <= low_threshold)
      set_state(LOW);
    else
      set_state(state);
  }

private:
  const std::function<float()> signal;
  const float low_threshold;
  const float high_threshold;
};
} // namespace DHE
#endif

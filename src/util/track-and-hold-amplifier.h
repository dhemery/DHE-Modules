#pragma once

#include <functional>
#include <utility>

namespace DHE {

/**
 * A track-and-hold amplifier (THA) operates in one of two modes: tracking and
 * holding. While tracking, a THA yields the supplied signal. While holding, it
 * yields the signal value it sampled when it entered holding mode.
 *
 * A newly constructed THA is holding.
 */
struct TrackAndHoldAmplifier {

  /**
   * Creates a track-and-hold amplifier that tracks and holds the supplied signal.
   * @param signal supplies the signal to track and hold
   */
  explicit TrackAndHoldAmplifier(std::function<float()> signal) :
      signal{std::move(signal)},
      held{this->signal()},
      tracking{false} {}

  /**
   * Enters holding mode, holding the current value of the supplied signal.
   */
  void hold() {
    held = signal();
    tracking = false;
  }

  /**
   * Enters tracking mode.
   */
  void track() {
    tracking = true;
  }

  /**
   * Returns the supplied signal value if the THA is tracking, and the held
   * value if it is holding.
   * @return the supplied signal value if the THA is tracking,
   * otherwise the held value.
   */
  float value() const {
    return tracking ? signal() : held;
  }

private:
  const std::function<float()> signal;
  float held;
  bool tracking;
};
}
#pragma once

namespace dhe {
/** A PhaseRotor is a phase accumulator that wraps the phase to the range [0,1).
 */
class PhaseRotor {
public:
  /** Advances the phase by the given amount, wrapping the result to the range
   * [0,1).
   */
  void advance(float delta);

  /** Returns the sine of the accumulated phase.
   */
  auto sin(float offset = 0.F) const -> float;

  /** Returns the cosine of the accumulated phase.
   */
  auto cos(float offset = 0.F) const -> float;

private:
  float phase_{0.F};
};

} // namespace dhe

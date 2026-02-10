#pragma once

namespace dhe {
/** A PhaseTimer is a phase accumulator that advances to a maximum of 1.
 */
class PhaseTimer {
public:
  PhaseTimer(float phase = 0.F);

  /** Advances the phase by the given amount, to a maximum of 1.
   */
  void advance(float delta);

  /** Returns whether the phase has advanced to 1.
   */
  auto in_progress() const -> bool;

  /** Returns the accumulated phase.
   */
  auto phase() const -> float;

  /** Resets the phase to 0.
   */
  void reset();

private:
  float phase_{};
};

} // namespace dhe

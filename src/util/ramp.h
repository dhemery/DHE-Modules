#ifndef DHE_UTIL_RAMP_H
#define DHE_UTIL_RAMP_H

#include <functional>
#include <utility>

#include "d-latch.h"
#include "sigmoid.h"

namespace DHE {

/**
 * A ramp that advances its phase from 0 to 1 in increments.
 *
 * When the phase advances to 1, the ramp fires endOfCycle and stops advancing.
 */
class Ramp {
public:
  /*!
   * Constructs a ramp that obtains the phase increment for each step by
   * calling the given supplier.
   *
   * The newly constructed ramp is at phase 0 and inactive.
   *
   * @param phaseIncrementSupplier supplies the amount to advance the phase on
   * each step
   */
  explicit Ramp(std::function<float()> phaseIncrementSupplier)
      : phaseIncrement{std::move(phaseIncrementSupplier)} {
    stop();
  }

  /*!
   * Constructs a ramp that advances its phase in steps of size:
   *
   *     ramp duration / step duration
   *
   * where the ramp duration and step duration are supplied by the suppliers.
   *
   * The newly constructed ramp is at phase 0 and inactive.
   *
   * @param rampDurationSupplier supplies the duration of the ramp
   * @param stepDurationSupplier supplies the duration of each step
   */
  Ramp(const std::function<float()> &rampDurationSupplier, const std::function<float()> &stepDurationSupplier)
      : Ramp{[=] { return stepDurationSupplier()/rampDurationSupplier(); }} {
  }

  /*!
   * Constructs a ramp that advances its phase in steps of size:
   *
   *     ramp duration / step duration
   *
   * with the given ramp duration and the step duration supplied by the
   * supplier.
   *
   * The newly constructed ramp is at phase 0 and inactive.
   *
   * @param rampDuration the duration of the ramp
   * @param stepDurationSupplier supplies the duration of each step
   */
  Ramp(float rampDuration, const std::function<float()> &stepDurationSupplier)
      : Ramp{[=] { return stepDurationSupplier()/rampDuration; }} {}

  /**
   * Activates the ramp at phase 0.
   *
   * Subsequent calls to step() will advance the phase.
   */
  void start() {
    progress = 0.0;
    active.resumeFiring();
    active.set();
  }

  /*!
   * Deactivates the ramp at phase 0.
   *
   * Subsequent calls to step() will not advance the phase.
   *
   * Stopping the ramp does not fire an endOfCycle event.
   */
  void stop() {
    progress = 0.0;
    active.suspendFiring();
    active.reset();
  }

  /**
   * Advances the phase at the ramp's current rate.
   *
   * If the phase advances to 1, the ramp fires endOfCycle and becomes inactive.
   *
   * If the ramp is inactive, this function has no effect.
   */
  void step() {
    if (!isActive())
      return;

    progress = NORMAL.clamp(progress + phaseIncrement());

    if (progress >= 1.0f) {
      active.reset();
    };
  }

  /**
   * Indicates whether the ramp is active.
   *
   * @return whether the ramp is active
   */
  bool isActive() const { return active.isHigh(); }

  /**
   * Returns the ramp's phase.
   *
   * If the ramp is active, the phase indicates the ramp's progress
   * from 0 to 1.
   *
   * If the ramp is inactive, the phase is 1 (if the ramp became inactive by
   * completing its advancement) or 0 (if the ramp was stopped).
   *
   * @return the ramp's phase
   */
  float phase() const { return progress; }

  /**
   * Registers an action to be called when the ramp's phase advances to 1.
   * @param action called when the ramp phase advances to 1
   */
  void onEndOfCycle(const std::function<void()> &action) {
    active.onFallingEdge(action);
  }

private:
  float progress = 0.0f;
  DLatch active{};
  const std::function<float()> phaseIncrement;
};
} // namespace DHE
#endif

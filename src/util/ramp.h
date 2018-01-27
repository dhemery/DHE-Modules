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
   * @param phase_increment_supplier supplies the amount to advance the phase on
   * each step
   */
  explicit Ramp(std::function<float()> phase_increment_supplier)
      : phase_increment{std::move(phase_increment_supplier)} {
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
   * @param ramp_duration_supplier supplies the duration of the ramp
   * @param step_duration_supplier supplies the duration of each step
   */
  Ramp(const std::function<float()> &ramp_duration_supplier, const std::function<float()> &step_duration_supplier)
      : Ramp{[=] { return step_duration_supplier()/ramp_duration_supplier(); }} {
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
   * @param ramp_duration the duration of the ramp
   * @param step_duration_supplier supplies the duration of each step
   */
  Ramp(float ramp_duration, const std::function<float()> &step_duration_supplier)
      : Ramp{[=] { return step_duration_supplier()/ramp_duration; }} {}

  /**
   * Activates the ramp at phase 0.
   *
   * Subsequent calls to step() will advance the phase.
   */
  void start() {
    progress = 0.0;
    active.resume_firing();
    active.set();
  }

  /*!
   * Deactivates the ramp at phase 0.
   *
   * Subsequent calls to step() will not advance the phase.
   *
   * Stopping the ramp does not fire an end-of-cycle event.
   */
  void stop() {
    progress = 0.0;
    active.suspend_firing();
    active.reset();
  }

  /**
   * Advances the phase at the ramp's current rate.
   *
   * If the phase advances to 1, the ramp fires end-of-cycle and becomes inactive.
   *
   * If the ramp is inactive, this function has no effect.
   */
  void step() {
    if (!is_active())
      return;

    progress = NORMAL.clamp(progress + phase_increment());

    if (progress >= 1.0f) {
      active.reset();
    };
  }

  /**
   * Indicates whether the ramp is active.
   *
   * @return whether the ramp is active
   */
  bool is_active() const { return active.is_high(); }

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
  void on_end_of_cycle(const std::function<void()> &action) {
    active.on_falling_edge(action);
  }

private:
  float progress = 0.0f;
  DLatch active{};
  const std::function<float()> phase_increment;
};
} // namespace DHE
#endif

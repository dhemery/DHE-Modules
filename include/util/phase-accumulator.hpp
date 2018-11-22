#pragma once

#include <functional>

#include "d-latch.hpp"
#include "sigmoid.hpp"

namespace DHE {

/**
 * Accumulates phase from 0 to 1 in increments supplied by a given supplier.
 *
 * When the phase advances to 1, the accumulator fires 'completion' and stops
 * advancing.
 */
class PhaseAccumulator {
public:
  /*!
   * Constructs an accumulator that advances its phase on each step by the
   * amount supplied by the given supplier.
   *
   * The newly constructed accumulator is at phase 0 and inactive.
   *
   * @param delta_supplier supplies the amount to advance the phase on each step
   */
  template <typename Supplier>
  explicit PhaseAccumulator(Supplier const &delta_supplier)
      : delta_{delta_supplier} {
    stop();
  }

  /**
   * Sets the phase to 0, actives the accumulator, and fires 'start'. Subsequent
   * steps will advance the phase.
   */
  void start() {
    phase_ = 0.0;
    active_.enable();
    active_.set();
  }

  /*!
   * Sets the phase to 0 and deactives the accumulator. Subsequent steps will
   * not advance the phase.
   *
   * Stopping the accumulator does not fire any events.
   */
  void stop() {
    phase_ = 0.0;
    active_.disable();
    active_.reset();
  }

  /**
   * Advances the phase by the amount supplied by the phase increment supplier.
   *
   * If the phase advances to 1, the accumulator fires 'completion' and becomes
   * inactive.
   *
   * If the accumulator is inactive, this function has no effect.
   */
  void step() {
    auto constexpr phase_range{Range{0.f, 1.f}};
    if (!is_active())
      return;

    phase_ = phase_range.clamp(phase_ + delta_());

    if (phase_ >= 1.0f) {
      active_.reset();
    };
  }

  /**
   * Indicates whether the accumulator is active.
   *
   * @return whether the accumulator is active
   */
  bool is_active() const { return active_.is_high(); }

  /**
   * Returns the accumulator's phase.
   *
   * If the accumulator is active, the phase indicates the accumulator's
   * progress from 0 to 1.
   *
   * If the accumulator is inactive, the phase is 1 (if the accumulator became
   * inactive by completing its advancement) or 0 (if the accumulator was
   * stopped).
   *
   * @return the accumulator's phase
   */
  float phase() const { return phase_; }

  /**
   * Registers an action to be called when the accumulator starts.
   * @param action called when the accumulator starts
   */
  template <typename Action> void on_start(Action const &action) {
    active_.on_rise(action);
  }

  /**
   * Registers an action to be called when the accumulator's phase advances
   * to 1.
   * @param action called when the accumulator's phase advances to 1
   */
  template <typename Action> void on_completion(Action const &action) {
    active_.on_fall(action);
  }

private:
  float phase_{0.0f};
  DLatch active_{};
  std::function<float()> const delta_;
};
} // namespace DHE

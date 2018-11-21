#pragma once

#include <functional>
#include <utility>

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
  explicit PhaseAccumulator(Supplier delta_supplier)
      : delta{std::move(delta_supplier)} {
    stop();
  }

  /**
   * Sets the phase to 0, actives the accumulator, and fires 'start'. Subsequent
   * steps will advance the phase.
   */
  void start() {
    _phase = 0.0;
    active.enable();
    active.set();
  }

  /*!
   * Sets the phase to 0 and deactives the accumulator. Subsequent steps will
   * not advance the phase.
   *
   * Stopping the accumulator does not fire any events.
   */
  void stop() {
    _phase = 0.0;
    active.disable();
    active.reset();
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
    constexpr auto phase_range = Range{0.f, 1.f};
    if (!is_active())
      return;

    _phase = phase_range.clamp(_phase + delta());

    if (_phase >= 1.0f) {
      active.reset();
    };
  }

  /**
   * Indicates whether the accumulator is active.
   *
   * @return whether the accumulator is active
   */
  bool is_active() const { return active.is_high(); }

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
  float phase() const { return _phase; }

  /**
   * Registers an action to be called when the accumulator starts.
   * @param action called when the accumulator starts
   */
  template <typename Action> void on_start(Action &&action) {
    active.on_rise(std::forward<Action>(action));
  }

  /**
   * Registers an action to be called when the accumulator's phase advances
   * to 1.
   * @param action called when the accumulator's phase advances to 1
   */
  template <typename Action> void on_completion(Action &&action) {
    active.on_fall(std::forward<Action>(action));
  }

private:
  float _phase = 0.0f;
  DLatch active{};
  const std::function<float()> delta;
};
} // namespace DHE

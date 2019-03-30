#pragma once
#include <algorithm>

namespace DHE {

/**
 * Advances a "phase" value from 0 to 1 over time.
 */
class PhaseAccumulator {
public:
  /**
   * Sets the phase to 0 and generates an on_start event.
   */
  void start() {
    phase = 0.f;
    on_start();
  }

  /**
   * Advances the phase according to the current duration and sample time.
   * If the phase advances to or beyond 1, this accumulator generates an
   * on_finish event and stops.
   */
  void step() {
    if (phase >= 1.f) {
      return;
    };
    phase = std::min(1.f, phase + sample_time() / duration());
    on_advance(phase);
    if (phase >= 1.f) {
      on_finish();
    };
  }

  void stop() { phase = 1.f; }

protected:
  virtual auto sample_time() const -> float = 0;
  virtual auto duration() const -> float = 0;
  virtual void on_start() const {};
  virtual void on_advance(float phase) const {};
  virtual void on_finish() const {};

private:
  float phase{0.f};
};
} // namespace DHE

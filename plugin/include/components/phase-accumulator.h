#pragma once

namespace DHE {

/**
 * Advances a "phase" value from 0 to 1 over time.
 */
class PhaseAccumulator {
public:
  PhaseAccumulator() {}

  /**
   * Sets the phase to 0 and generates an on_start event.
   */
  void start() {
    accumulated = 0.f;
    on_start();
  }

  /**
   * Advances the phase according to the current duration and sample time.
   * If the phase advances to or beyond 1, this accumulator generates an
   * on_finish event.
   */
  void step() {
    accumulated += sampleTime() / duration();
    if (accumulated >= 1.0f) {
      accumulated = 1.f;
    };
    on_step(accumulated);
    if (accumulated >= 1.0f) {
      on_finish();
    };
  }

  /**
   * Returns the currently accumulated phase.
   */
  auto phase() const -> float { return this->accumulated; }

protected:
  virtual auto sampleTime() const -> float = 0;
  virtual auto duration() const -> float = 0;
  virtual void on_start() const {};
  virtual void on_step(float phase) const {};
  virtual void on_finish() const {};

private:
  float accumulated = 0.f;
};
} // namespace DHE

#pragma once
#include <algorithm>
#include <utility>

namespace dhe {

/**
 * Advances a "phase" value from 0 to 1 over time.
 */
class PhaseAccumulator {
public:
  explicit PhaseAccumulator(std::function<float()> duration, std::function<void()> onStart,
                            std::function<void(float)> onAdvance, std::function<void()> onFinish) :
      duration{std::move(duration)},
      onStart{std::move(onStart)},
      onAdvance{std::move(onAdvance)},
      onFinish{std::move(onFinish)} {}
  /**
   * Sets the phase to 0 and generates an on_start event.
   */
  void start() {
    phase = 0.F;
    onStart();
  }

  /**
   * Advances the phase according to the current duration and sample time.
   * If the phase advances to or beyond 1, this accumulator generates an
   * on_finish event and stops.
   */
  void step(float sampleTime) {
    if (phase >= 1.F) {
      return;
    }
    phase = std::min(1.F, phase + sampleTime / duration());
    onAdvance(phase);
    if (phase >= 1.F) {
      onFinish();
    }
  }

  void stop() { phase = 1.F; }

protected:
private:
  float phase{0.F};
  const std::function<float()> duration;
  const std::function<void()> onStart;
  const std::function<void(float)> onAdvance;
  const std::function<void()> onFinish;
};
} // namespace dhe

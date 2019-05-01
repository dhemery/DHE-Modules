#pragma once
#include <algorithm>
#include <utility>

namespace DHE {

/**
 * Advances a "phase" value from 0 to 1 over time.
 */
class PhaseAccumulator {
public:
  explicit PhaseAccumulator(std::function<float()> duration,
                            std::function<float()> sample_time,
                            std::function<void()> on_start,
                            std::function<void(float)> on_advance,
                            std::function<void()> on_finish)
      : duration{std::move(duration)},
        sample_time{std::move(sample_time)}, on_start{std::move(on_start)},
        on_advance{std::move(on_advance)}, on_finish{std::move(on_finish)} {}
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
private:
  float phase{0.f};
  const std::function<float()> duration;
  const std::function<float()> sample_time;
  const std::function<void()> on_start;
  const std::function<void(float)> on_advance;
  const std::function<void()> on_finish;
};
} // namespace DHE

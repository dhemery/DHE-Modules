#include <utility>

#pragma once
#include <functional>
#include <utility>

#include "components/phase-accumulator.h"

namespace DHE {

/**
 * Generates a 1ms end-of-cycle pulse. The pulse generator calls on_eoc_rise()
 * when the pulse starts and on_eoc_fall() when the pulse ends.
 */
template <typename M> class EndOfCyclePulseGenerator : public PhaseAccumulator {
public:
  explicit EndOfCyclePulseGenerator(const std::function<float()> &sample_time,
                                    std::function<void()> on_eoc_rise,
                                    std::function<void()> on_eoc_fall)
      : PhaseAccumulator{sample_time}, on_eoc_rise{std::move(on_eoc_rise)},
        on_eoc_fall{std::move(on_eoc_fall)} {}

  void on_start() const override { on_eoc_rise(); }

  auto duration() const -> float override { return 1e-3; }

  void on_finish() const override { on_eoc_fall(); }

private:
  const std::function<void()> on_eoc_rise;
  const std::function<void()> on_eoc_fall;
};
} // namespace DHE

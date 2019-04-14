#pragma once

#include <functional>
#include <utility>

#include "components/phase-accumulator.h"

namespace DHE {

/**
 * Generates a 1ms end-of-cycle pulse. The pulse generator calls on_eoc_rise()
 * when the pulse starts and on_eoc_fall() when the pulse ends.
 */
class EndOfCyclePulseGenerator : public PhaseAccumulator {
public:
  EndOfCyclePulseGenerator(std::function<float()> sample_time,
                           std::function<void()> on_eoc_rise,
                           std::function<void()> on_eoc_fall)
      : PhaseAccumulator{[]() { return 1e-3; }, std::move(sample_time),
                         std::move(on_eoc_rise), [](float) {},
                         std::move(on_eoc_fall)} {}
};
} // namespace DHE

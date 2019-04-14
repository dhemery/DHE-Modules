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
class EndOfCyclePulseGenerator : public PhaseAccumulator {
public:
  EndOfCyclePulseGenerator(const std::function<float()> &sample_time,
                                    const std::function<void()>& on_eoc_rise,
                                    const std::function<void()>& on_eoc_fall)
      : PhaseAccumulator{[]() { return 1e-3; }, sample_time, on_eoc_rise,
                         [](float) {}, on_eoc_fall} {}
};
} // namespace DHE

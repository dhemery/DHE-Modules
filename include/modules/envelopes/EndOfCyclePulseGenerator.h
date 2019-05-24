#pragma once
#include "modules/components/PhaseAccumulator.h"

#include <functional>
#include <utility>

namespace dhe {

static float constexpr oneMillisecond = 1e-3;

/**
 * Generates a 1ms end-of-cycle pulse. The pulse generator calls on_eoc_rise()
 * when the pulse starts and on_eoc_fall() when the pulse ends.
 */
class EndOfCyclePulseGenerator : public PhaseAccumulator {
public:
  EndOfCyclePulseGenerator(std::function<void()> onEocRise, std::function<void()> onEocFall) :
      PhaseAccumulator{[]() { return oneMillisecond; }, std::move(onEocRise), [](float) {}, std::move(onEocFall)} {}
};
} // namespace dhe

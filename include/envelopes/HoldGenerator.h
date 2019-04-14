#pragma once

#include <functional>
#include <utility>

#include "components/phase-accumulator.h"

namespace DHE {
class HoldGenerator : public PhaseAccumulator {
public:
  HoldGenerator(std::function<float()> duration,
                const std::function<float()> &sample_time,
                const std::function<void()> &on_hold_complete)
      : PhaseAccumulator{std::move(duration), sample_time, []() {},
                         [](float phase) {}, on_hold_complete} {}
};
} // namespace DHE

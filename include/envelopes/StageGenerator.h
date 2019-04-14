#pragma once

#include "components/phase-accumulator.h"

namespace DHE {

class StageGenerator : public PhaseAccumulator {
public:
  StageGenerator(const std::function<float()> &duration,
                 const std::function<float()> &sample_time,
                 const std::function<void(float)> &generate,
                 const std::function<void()> &on_stage_complete)
      : PhaseAccumulator{duration, sample_time, []() {}, generate,
                         on_stage_complete} {}
};
} // namespace DHE

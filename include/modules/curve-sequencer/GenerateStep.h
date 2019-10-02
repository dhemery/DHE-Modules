#pragma once

#include "Step.h"
#include "StepControls.h"

namespace dhe {
namespace curve_sequencer {
  class GenerateStep : public Step {
  public:
    GenerateStep(StepControls &controls, int stepIndex);
    auto isAvailable() const -> bool override;
    void process(float sampleTime) override;

  private:
    StepControls &controls;
    int stepIndex;
  };
} // namespace curve_sequencer
} // namespace dhe
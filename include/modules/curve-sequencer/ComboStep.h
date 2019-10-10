#pragma once

#include "Step.h"
#include "StepControls.h"

#include <memory>

namespace dhe {

namespace curve_sequencer {

  class ComboStep : public Step {
  public:
    ComboStep(StepControls &controls, int stepIndex);
    // Constructor for testing
    ComboStep(StepControls &controls, int stepIndex, Step *generateStep, Step *sustainStep);

    auto index() const -> int override { return stepIndex; }
    auto isAvailable() const -> bool override;
    auto process(Latch const &gateLatch, float sampleTime) -> State override;

  private:
    StepControls &controls;
    int stepIndex;
    std::unique_ptr<Step> generateStep;
    std::unique_ptr<Step> sustainStep;
  };
} // namespace curve_sequencer
} // namespace dhe

#pragma once

#include "Step.h"
#include "StepControls.h"

#include <memory>

namespace dhe {

namespace curve_sequencer {

  class ComboStep : public Step {
  public:
    ComboStep(StepControls &controls, int stepIndex);
    auto isAvailable() const -> bool override;
    void process(Latch const &gateLatch, float sampleTime) override;

    // Constructor for testing
    ComboStep(StepControls &controls, int stepIndex, Step *generateStep, Step *sustainStep);

  private:
    StepControls &controls;
    int stepIndex;
    std::unique_ptr<Step> generateStep;
    std::unique_ptr<Step> sustainStep;
  };
} // namespace curve_sequencer
} // namespace dhe

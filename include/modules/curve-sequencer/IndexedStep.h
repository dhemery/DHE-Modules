#pragma once

#include "Step.h"
#include "StepControls.h"

namespace dhe {

namespace curve_sequencer {

  class IndexedStep : public Step {
  public:
    enum Mode { Rise, Fall, Edge, High, Low, Skip, Duration };

    IndexedStep(StepControls &controls, int index) : controls{controls}, index{index} {}

    auto isAvailable() const -> bool override { return controls.isEnabled(index); }

    void process(float /*sampleTime*/) override {
      if (controls.generateMode(index) != Skip) {
        controls.setGenerating(index, true);
      } else if (controls.sustainMode(index) != Skip) {
        controls.setSustaining(index, true);
      }
    }

  private:
    StepControls &controls;
    int index;
  };
} // namespace curve_sequencer
} // namespace dhe

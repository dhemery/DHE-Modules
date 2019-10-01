#pragma once

#include "Step.h"

namespace dhe {

namespace curve_sequencer {

  template <typename M> class ModuleStep : public Step {
  public:
    ModuleStep(M *module, int stepIndex) : module{module}, stepIndex{stepIndex} {}
    ~ModuleStep() override = default;

    enum Mode { Rise, Fall, Edge, High, Low, Skip, Duration };

    auto isAvailable() const -> bool override { return module->isEnabled(stepIndex); }
    void process(float /*sampleTime*/) override {
      if (module->generateMode(stepIndex) != Skip) {
        module->setGenerating(stepIndex, true);
      } else if (module->sustainMode(stepIndex) != Skip) {
        module->setSustaining(stepIndex, true);
      }
    }

  private:
    M *module;
    int stepIndex;
  };
} // namespace curve_sequencer
} // namespace dhe

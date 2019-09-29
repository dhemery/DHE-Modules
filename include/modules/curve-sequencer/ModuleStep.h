#pragma once

#include "Step.h"

namespace dhe {

namespace curve_sequencer {

  template <typename M> class ModuleStep : public Step {
  public:
    ModuleStep(M *module, int stepIndex) : module{module}, stepIndex{stepIndex} {}
    ~ModuleStep() override = default;

    auto isAvailable() const -> bool override { return module->isEnabled(stepIndex); }
    void process(float /*sampleTime*/) override { module->setGenerating(stepIndex, true); }

  private:
    M *module;
    int stepIndex;
  };
} // namespace curve_sequencer
} // namespace dhe

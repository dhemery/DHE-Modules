#pragma once

#include "components/latch.h"
#include "components/phase-timer.h"

namespace dhe {
namespace shape_scanner {

template <typename Module, typename StepSelector, typename StepController>
class SequenceController {
public:
  SequenceController(Module &module, StepSelector &step_selector,
                     StepController &step_controller)
      : module_{module}, step_selector_{step_selector}, step_controller_{
                                                            step_controller} {}

  void execute() {}

private:
  void generate() {
    auto phase = 0.F;
    step_controller_.execute(phase);
  }

  int step_{-1};
  Module &module_;
  StepSelector &step_selector_;
  StepController &step_controller_;
};
} // namespace shape_scanner
} // namespace dhe

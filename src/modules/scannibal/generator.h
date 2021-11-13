#pragma once

#include "components/range.h"
#include "signals/shapes.h"

namespace dhe {
namespace scannibal {
using dhe::Range;

template <typename Module, typename Anchor> class Generator {
public:
  Generator(Module &module, Anchor &phase_0_anchor, Anchor &phase_1_anchor)
      : module_{module}, phase_0_anchor_{phase_0_anchor}, phase_1_anchor_{
                                                              phase_1_anchor} {}

  void generate(int step, float phase) {
    if (step != previous_step_) {
      phase_0_anchor_.enter(step);
      phase_1_anchor_.enter(step);
      previous_step_ = step;
    }
    auto const curvature = module_.curvature(step);
    auto const phase_0_voltage = phase_0_anchor_.voltage();
    auto const phase_1_voltage = phase_1_anchor_.voltage();
    auto const range = Range{phase_0_voltage, phase_1_voltage};
    auto const shape = module_.shape(step);

    auto const out_voltage =
        range.scale(Shapes::taper(phase, shape, curvature));

    module_.output(out_voltage);
  }

private:
  Module &module_;
  Anchor &phase_0_anchor_;
  Anchor &phase_1_anchor_;
  int previous_step_{-1};
};
} // namespace scannibal
} // namespace dhe

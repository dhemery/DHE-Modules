#pragma once
#include "components/range.h"

namespace dhe {
namespace scannibal {
using dhe::Range;

template <typename Module, typename Anchor> class Generator {
public:
  Generator(Module &module, Anchor &start_anchor, Anchor &end_anchor)
      : module_{module}, start_anchor_{start_anchor}, end_anchor_{end_anchor} {}

  void generate(int step, float phase) {
    if (step != previous_step_) {
      start_anchor_.enter(step);
      end_anchor_.enter(step);
      previous_step_ = step;
    }
    auto const curvature = module_.curvature(step);
    auto const start_voltage = start_anchor_.voltage();
    auto const end_voltage = end_anchor_.voltage();
    auto const range = Range{start_voltage, end_voltage};
    auto const taper = module_.taper(step);

    auto const out_voltage = range.scale(taper.apply(phase, curvature));

    module_.output(out_voltage);
  }

private:
  Module &module_;
  Anchor &start_anchor_;
  Anchor &end_anchor_;
  int previous_step_{-1};
};
} // namespace scannibal
} // namespace dhe

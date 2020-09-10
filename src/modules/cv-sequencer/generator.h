#pragma once
#include "components/phase-timer.h"
#include "components/range.h"
#include "status.h"

namespace dhe {
namespace cv_sequencer {
using dhe::Range;

template <typename Module, typename Anchor> class Generator {
public:
  Generator(Module &module, Anchor &start_anchor, Anchor &end_anchor)
      : module_{module}, start_anchor_{start_anchor}, end_anchor_{end_anchor} {}

  void start(int step) {
    step_ = step;
    timer_.reset();
    module_.show_progress(step_, timer_.phase());
    start_anchor_.enter(step_);
    end_anchor_.enter(step_);
  }

  auto generate(float sample_time) -> GeneratorStatus {
    auto const duration = module_.duration(step_);
    auto const curvature = module_.curvature(step_);
    auto const start_voltage = start_anchor_.voltage();
    auto const end_voltage = end_anchor_.voltage();
    auto const range = Range{start_voltage, end_voltage};
    auto const taper = module_.taper(step_);

    timer_.advance(sample_time / duration);
    auto const phase = timer_.phase();
    auto const out_voltage = range.scale(taper.apply(phase, curvature));
    module_.output(out_voltage);
    module_.show_progress(step_, timer_.phase());
    return timer_.in_progress() ? GeneratorStatus::Generating
                                : GeneratorStatus::Completed;
  }

  void stop() { module_.show_inactive(step_); }

private:
  Module &module_;
  Anchor &start_anchor_;
  Anchor &end_anchor_;
  PhaseTimer timer_{};
  int step_{0};
};
} // namespace cv_sequencer
} // namespace dhe

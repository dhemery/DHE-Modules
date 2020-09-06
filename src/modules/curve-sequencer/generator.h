#pragma once
#include "components/phase-timer.h"
#include "status.h"

namespace dhe {
namespace curve_sequencer {

template <typename Module, typename Anchor> class Generator {
public:
  Generator(Module &module, Anchor &start_anchor, Anchor &end_anchor)
      : module_{module}, start_anchor_{start_anchor}, end_anchor_{end_anchor} {}

  void start(int step) {
    step_ = step;
    module_.show_progress(step_, 0.F);
    start_anchor_.enter(step_);
    end_anchor_.enter(step_);
  }

  auto generate(float /*sample_time*/) -> GeneratorStatus {
    return GeneratorStatus::Generating;
  }

  void stop() { module_.show_inactive(step_); }

private:
  Module &module_;
  Anchor &start_anchor_;
  Anchor &end_anchor_;
  PhaseTimer timer_{};
  int step_{0};
};
} // namespace curve_sequencer
} // namespace dhe

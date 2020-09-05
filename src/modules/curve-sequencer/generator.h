#pragma once
#include "components/phase-timer.h"
#include "generator-status.h"

namespace dhe {
namespace curve_sequencer {

template <typename Controls, typename Start, typename End> class Generator {
public:
  Generator(Controls &controls, Start &start_anchor, End &end_anchor)
      : controls_{controls}, start_anchor_{start_anchor}, end_anchor_{
                                                              end_anchor} {}

  void start(int step) {
    step_ = step;
    controls_.show_progress(step_, 0.F);
    start_anchor_.enter(step_);
    end_anchor_.enter(step_);
  }

  auto generate(float /*sample_time*/) -> GeneratorStatus {
    return GeneratorStatus::Generating;
  }

  void stop() { controls_.show_inactive(step_); }

private:
  Controls &controls_;
  Start &start_anchor_;
  End &end_anchor_;
  PhaseTimer timer_{};
  int step_{0};
};
} // namespace curve_sequencer
} // namespace dhe

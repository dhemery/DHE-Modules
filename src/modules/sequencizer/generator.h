#pragma once
#include "components/phase-timer.h"
#include "components/range.h"
#include "status.h"

namespace dhe {
namespace sequencizer {
using dhe::Range;

template <typename Controls, typename Anchor> class Generator {
public:
  Generator(Controls &controls, Anchor &start_anchor, Anchor &end_anchor)
      : controls_{controls}, start_anchor_{start_anchor}, end_anchor_{
                                                              end_anchor} {}

  void start(int step) {
    step_ = step;
    timer_.reset();
    controls_.show_progress(step_, timer_.phase());
    start_anchor_.enter(step_);
    end_anchor_.enter(step_);
  }

  auto generate(float sample_time) -> GeneratorStatus {
    auto const duration = controls_.duration(step_);
    auto const curvature = controls_.curvature(step_);
    auto const start_voltage = start_anchor_.voltage();
    auto const end_voltage = end_anchor_.voltage();
    auto const range = Range{start_voltage, end_voltage};
    auto const taper = controls_.taper(step_);

    timer_.advance(sample_time / duration);
    auto const phase = timer_.phase();
    auto const out_voltage = range.scale(taper.apply(phase, curvature));
    controls_.output(out_voltage);
    controls_.show_progress(step_, timer_.phase());
    return timer_.in_progress() ? GeneratorStatus::Generating
                                : GeneratorStatus::Completed;
  }

  void stop() { controls_.show_inactive(step_); }

private:
  Controls &controls_;
  Anchor &start_anchor_;
  Anchor &end_anchor_;
  PhaseTimer timer_{};
  int step_{0};
};
} // namespace sequencizer
} // namespace dhe

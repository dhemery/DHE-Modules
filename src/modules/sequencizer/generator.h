#pragma once

#include "components/phase-timer.h"
#include "components/range.h"
#include "components/sigmoid.h"
#include "status.h"

namespace dhe {
namespace sequencizer {
using dhe::Range;

template <typename Signals, typename Anchor> class Generator {
public:
  Generator(Signals &signals, Anchor &start_anchor, Anchor &end_anchor)
      : signals_{signals}, start_anchor_{start_anchor}, end_anchor_{
                                                            end_anchor} {}

  void start(int step) {
    step_ = step;
    timer_.reset();
    signals_.show_progress(step_, timer_.phase());
    start_anchor_.enter(step_);
    end_anchor_.enter(step_);
  }

  auto generate(float sample_time) -> GeneratorStatus {
    auto const duration = signals_.duration(step_);
    auto const curvature = signals_.curvature(step_);
    auto const start_voltage = start_anchor_.voltage();
    auto const end_voltage = end_anchor_.voltage();
    auto const range = Range{start_voltage, end_voltage};
    auto const shape = signals_.shape(step_);

    timer_.advance(sample_time / duration);
    auto const phase = timer_.phase();
    auto const out_voltage =
        range.scale(sigmoid::Shape::apply(shape, phase, curvature));
    signals_.output(out_voltage);
    signals_.show_progress(step_, phase);
    return timer_.in_progress() ? GeneratorStatus::Generating
                                : GeneratorStatus::Completed;
  }

  void stop() { signals_.show_inactive(step_); }

private:
  Signals &signals_;
  Anchor &start_anchor_;
  Anchor &end_anchor_;
  PhaseTimer timer_{};
  int step_{0};
};
} // namespace sequencizer
} // namespace dhe

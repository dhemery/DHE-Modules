#pragma once

#include "advance-mode.h"
#include "components/latch.h"
#include "components/phase-timer.h"
#include "components/range.h"
#include "generate-mode.h"
#include "step-event.h"

namespace dhe {
namespace curve_sequencer {

using dhe::Latch;
using dhe::PhaseTimer;

static inline auto is_satisfied(GenerateMode mode, AdvanceMode condition,
                                Latch const &gate) -> bool {
  switch (condition) {
  case AdvanceMode::GateIsHigh:
    return gate.is_high();
  case AdvanceMode::GateIsLow:
    return gate.is_low();
  case AdvanceMode::GateRises:
    return gate.is_rise();
  case AdvanceMode::GateFalls:
    return gate.is_fall();
  case AdvanceMode::GateChanges:
    return gate.is_edge();
  case AdvanceMode::TimerExpires:
  default:
    return mode == GenerateMode::Sustain;
  }
}

template <typename Controls> class StepController {
public:
  StepController(Controls &controls, PhaseTimer &timer)
      : controls_{controls}, timer_{timer} {}

  void enter(int step) {
    step_ = step;
    reset();
    controls_.show_progress(step_, 0.F);
  }

  auto execute(Latch const &gate, float sample_time) -> StepEvent {
    GenerateMode step_mode = mode();
    if (!is_satisfied(step_mode, condition(), gate)) {
      if (step_mode != GenerateMode::Sustain) { // Sustain has no duration
        timer_.advance(sample_time / duration());
      }
      controls_.show_progress(step_, timer_.phase());
      switch (step_mode) {
      case GenerateMode::Curve:
        controls_.output(scale(taper(timer_.phase()), start_voltage_, level()));
        break;
      case GenerateMode::Input:
        controls_.output(controls_.input());
        break;
      case GenerateMode::Chase:
        controls_.output(
            scale(taper(timer_.phase()), start_voltage_, controls_.input()));
        break;
      case GenerateMode::Level:
        controls_.output(level());
        break;
      default: // Hold and sustain
        break;
      }
      if (timer_.in_progress()) {
        return StepEvent::Generated;
      }
    }
    // This step is done, because either the gate satisfied the condition or
    // advancing the phase completed it.
    exit();
    return StepEvent::Completed;
  };

  void exit() { controls_.show_inactive(step_); }

private:
  auto condition() const -> AdvanceMode { return controls_.condition(step_); }

  auto duration() const -> float { return controls_.duration(step_); }

  auto level() const -> float { return controls_.level(step_); }

  auto mode() const -> GenerateMode { return controls_.mode(step_); }

  void reset() {
    timer_.reset();
    start_voltage_ = controls_.output();
  }

  auto taper(float input) const -> float {
    auto const curvature = controls_.curvature(step_);
    auto const taper = controls_.taper(step_);
    return taper->apply(input, curvature);
  }

  int step_{0};
  float start_voltage_{0.F};
  Controls &controls_;
  PhaseTimer &timer_;
}; // namespace curve_sequencer
} // namespace curve_sequencer
} // namespace dhe

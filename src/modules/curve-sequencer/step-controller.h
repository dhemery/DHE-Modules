#pragma once

#include "advance-mode.h"
#include "generate-mode.h"
#include "step-event.h"

#include "components/cxmath.h"
#include "components/latch.h"
#include "components/phase-timer.h"
#include "components/range.h"
#include "components/sigmoid.h"

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

template <typename Signals> class StepController {
public:
  StepController(Signals &signals, PhaseTimer &timer)
      : signals_{signals}, timer_{timer} {}

  void enter(int step) {
    step_ = step;
    reset();
    signals_.show_progress(step_, 0.F);
  }

  auto execute(Latch const &gate, float sample_time) -> StepEvent {
    GenerateMode step_mode = generate_mode();
    if (!is_satisfied(step_mode, advance_mode(), gate)) {
      if (step_mode != GenerateMode::Sustain) { // Sustain has no duration
        timer_.advance(sample_time / duration());
      }
      signals_.show_progress(step_, timer_.phase());
      switch (step_mode) {
      case GenerateMode::Curve:
        signals_.output(
            cx::scale(taper(timer_.phase()), start_voltage_, level()));
        break;
      case GenerateMode::Input:
        signals_.output(signals_.input());
        break;
      case GenerateMode::Chase:
        signals_.output(
            cx::scale(taper(timer_.phase()), start_voltage_, signals_.input()));
        break;
      case GenerateMode::Level:
        signals_.output(level());
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

  void exit() { signals_.show_inactive(step_); }

private:
  auto advance_mode() const -> AdvanceMode {
    return signals_.advance_mode(step_);
  }

  auto duration() const -> float { return signals_.duration(step_); }

  auto generate_mode() const -> GenerateMode {
    return signals_.generate_mode(step_);
  }

  auto level() const -> float { return signals_.level(step_); }

  void reset() {
    timer_.reset();
    start_voltage_ = signals_.output();
  }

  auto taper(float input) const -> float {
    auto const curvature = signals_.curvature(step_);
    auto const shape = signals_.shape(step_);
    return sigmoid::Shape::apply(shape, input, curvature);
  }

  int step_{0};
  float start_voltage_{0.F};
  Signals &signals_;
  PhaseTimer &timer_;
}; // namespace curve_sequencer
} // namespace curve_sequencer
} // namespace dhe

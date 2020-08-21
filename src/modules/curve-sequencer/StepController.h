#pragma once

#include "AdvanceMode.h"
#include "GenerateMode.h"
#include "StepEvent.h"
#include "components/latch.h"
#include "components/phase-timer.h"
#include "components/range.h"

namespace dhe {
namespace curve_sequencer {

using dhe::Latch;
using dhe::PhaseTimer;

static inline auto isSatisfied(GenerateMode mode, AdvanceMode condition,
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
      : controls{controls}, timer{timer} {}

  void enter(int entryStep) {
    step = entryStep;
    reset();
    controls.showProgress(step, 0.F);
  }

  auto execute(Latch const &gateLatch, float sampleTime) -> StepEvent {
    GenerateMode stepMode = mode();
    if (!isSatisfied(stepMode, condition(), gateLatch)) {
      if (stepMode != GenerateMode::Sustain) { // Sustain has no duration
        timer.advance(sampleTime / duration());
      }
      controls.showProgress(step, timer.phase());
      switch (stepMode) {
      case GenerateMode::Curve:
        controls.output(scale(taper(timer.phase()), startVoltage, level()));
        break;
      case GenerateMode::Input:
        controls.output(controls.input());
        break;
      case GenerateMode::Chase:
        controls.output(
            scale(taper(timer.phase()), startVoltage, controls.input()));
        break;
      case GenerateMode::Level:
        controls.output(level());
        break;
      default: // Hold and sustain
        break;
      }
      if (timer.in_progress()) {
        return StepEvent::Generated;
      }
    }
    // This step is done, because either the gate satisfied the condition or
    // advancing the phase completed it.
    exit();
    return StepEvent::Completed;
  };

  void exit() { controls.showInactive(step); }

private:
  auto condition() const -> AdvanceMode { return controls.condition(step); }

  auto duration() const -> float { return controls.duration(step); }

  auto level() const -> float { return controls.level(step); }

  auto mode() const -> GenerateMode { return controls.mode(step); }

  void reset() {
    timer.reset();
    startVoltage = controls.output();
  }

  auto taper(float input) const -> float {
    auto const curvature = controls.curvature(step);
    auto const taper = controls.taper(step);
    return taper->apply(input, curvature);
  }

  int step{0};
  float startVoltage{0.F};
  Controls &controls;
  PhaseTimer &timer;
}; // namespace curve_sequencer
} // namespace curve_sequencer
} // namespace dhe

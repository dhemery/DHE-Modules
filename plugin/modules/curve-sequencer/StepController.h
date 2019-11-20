#pragma once

#include "AdvanceCondition.h"
#include "StepEvent.h"
#include "StepMode.h"
#include "components/Latch.h"
#include "components/Range.h"

namespace dhe {
namespace curve_sequencer {

  static inline auto isSatisfied(StepMode mode, AdvanceCondition condition, dhe::Latch const &gate) -> bool {
    switch (condition) {
    case AdvanceCondition::GateIsHigh:
      return gate.isHigh();
    case AdvanceCondition::GateIsLow:
      return gate.isLow();
    case AdvanceCondition::GateRises:
      return gate.isRise();
    case AdvanceCondition::GateFalls:
      return gate.isFall();
    case AdvanceCondition::GateChanges:
      return gate.isEdge();
    case AdvanceCondition::TimerExpires:
    default:
      return mode == StepMode::Sustain;
    }
  }

  template <typename Controls, typename PhaseAccumulator> class StepController {
  public:
    StepController(Controls &controls, PhaseAccumulator &phase) : controls{controls}, phase{phase} {}

    void enter(int entryStep) {
      step = entryStep;
      reset();
      showActive(true);
    }

    auto execute(dhe::Latch const &gateLatch, float sampleTime) -> StepEvent {
      StepMode stepMode = mode();
      if (!isSatisfied(stepMode, condition(), gateLatch)) {
        if (stepMode == StepMode::Sustain) {
          reset();
        } else {
          phase.advance(sampleTime / duration());
        }
        if (stepMode == StepMode::Curve) {
          controls.output(scale(taper(phase.phase()), startVoltage, level()));
        }
        if (phase.state() == PhaseAccumulator::State::Incomplete) {
          return StepEvent::Generated;
        }
      }
      showActive(false);
      return StepEvent::Completed;
    };

    void exit() { showActive(false); }

  private:
    auto condition() const -> AdvanceCondition { return controls.condition(step); }

    auto duration() const -> float { return controls.duration(step); }

    void generate(float sampleTime) {
      phase.advance(sampleTime / duration());
      controls.output(scale(taper(phase.phase()), startVoltage, level()));
    }

    auto level() const -> float { return controls.level(step); }

    auto mode() const -> StepMode { return controls.mode(step); }

    void reset() {
      phase.reset();
      startVoltage = controls.output();
    }

    void showActive(bool state) const { controls.showActive(step, state); }

    auto taper(float input) const -> float {
      auto const curvature = controls.curvature(step);
      auto const taper = controls.taper(step);
      return taper->apply(input, curvature);
    }

    int step{0};
    float startVoltage{0.F};
    Controls &controls;
    PhaseAccumulator &phase;
  }; // namespace curve_sequencer
} // namespace curve_sequencer
} // namespace dhe

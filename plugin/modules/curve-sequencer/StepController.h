#pragma once

#include "SequenceMode.h"
#include "StepCondition.h"
#include "components/Latch.h"
#include "components/Range.h"

namespace dhe {
namespace curve_sequencer {

  template <typename Controls, typename PhaseAccumulator> class StepController {
  public:
    StepController(Controls &controls, PhaseAccumulator &phase) : controls{controls}, phase{phase} {}

    auto isAvailable(int candidateStep, Latch const &gateLatch) const -> bool {
      if (!controls.isEnabled(candidateStep)) {
        return false;
      }
      auto const mode = controls.mode(candidateStep);
      auto const condition = controls.condition(candidateStep);
      return !isSatisfied(mode, condition, gateLatch);
    }

    void enter(int entryStep) {
      step = entryStep;
      phase.reset();
      startVoltage = controls.output();
      showActive(true);
    }

    auto execute(dhe::Latch const &gateLatch, float sampleTime) -> SequenceMode {
      StepMode stepMode = mode();
      if (!isSatisfied(stepMode, condition(), gateLatch)) {
        if(stepMode != StepMode::Sustain) {
          phase.advance(sampleTime / duration());
        }
        if(stepMode == StepMode::Curve) {
          controls.output(scale(taper(phase.phase()), startVoltage, level()));
        }
        if (phase.state() == PhaseAccumulator::State::Incomplete) {
          return SequenceMode::Generating;
        }
      }
      showActive(false);
      return SequenceMode::Advancing;
    };

    void exit() { showActive(false); }

  private:
    auto condition() const -> StepCondition { return controls.condition(step); }

    auto duration() const -> float { return controls.duration(step); }

    auto level() const -> float { return controls.level(step); }

    void generate(float sampleTime) {
      phase.advance(sampleTime / duration());
      controls.output(scale(taper(phase.phase()), startVoltage, level()));
    }

    auto mode() const -> StepMode { return controls.mode(step); }

    void showActive(bool state) const { controls.showActive(step, state); }

    auto taper(float input) const -> float {
      auto const curvature = controls.curvature(step);
      auto const taper = controls.taper(step);
      return taper->apply(input, curvature);
    }

    Controls &controls;
    PhaseAccumulator &phase;
    int step;
    float startVoltage;
  }; // namespace curve_sequencer
} // namespace curve_sequencer
} // namespace dhe

#pragma once

#include "SequenceMode.h"
#include "SequencerState.h"
#include "StageMode.h"
#include "components/Latch.h"
#include "components/OneShotPhaseAccumulator.h"
#include "components/Range.h"

namespace dhe {
namespace curve_sequencer {

  template <typename Controls, typename Phase = OneShotPhaseAccumulator> class GenerateStage {
  public:
    GenerateStage(Controls &controls, Phase &phase) : controls{controls}, phase{phase} {}

    void enter(int entryStep) {
      step = entryStep;
      phase.reset();
      startVoltage = controls.output();
      showActive(true);
    }

    auto execute(dhe::Latch const &gateLatch, float sampleTime) -> SequencerState {
      if (isActive(generateMode(), gateLatch)) {
        generate(sampleTime);
        if (phase.state() == OneShotPhaseAccumulator::State::Incomplete) {
          return {SequenceMode::Generating, step};
        }
      }
      showActive(false);
      return {SequenceMode::Sustaining, step};
    };

    void exit() { showActive(false); }

  private:
    auto duration() const -> float { return controls.duration(step); }

    auto level() const -> float { return controls.level(step); }

    void generate(float sampleTime) {
      phase.advance(sampleTime / duration());
      controls.output(scale(taper(phase.phase()), startVoltage, level()));
    }

    auto generateMode() const -> StageMode { return controls.generateMode(step); }

    void showActive(bool state) const { controls.showGenerating(step, state); }

    auto taper(float input) const -> float {
      auto const curvature = controls.curvature(step);
      auto const taper = controls.taper(step);
      return taper->apply(input, curvature);
    }

    Controls &controls;
    Phase &phase;
    int step;
    float startVoltage;
  }; // namespace curve_sequencer
};   // namespace curve_sequencer
} // namespace dhe

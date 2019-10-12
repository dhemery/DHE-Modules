#include "modules/curve-sequencer/GenerateStep.h"

#include "modules/components/Taper.h"
#include "modules/curve-sequencer/StepControls.h"

namespace dhe {

namespace curve_sequencer {
  static auto isActive(GenerateStep::Mode mode, Latch const &gateLatch) -> bool {
    switch (mode) {
    case GenerateStep::Mode::Duration:
      return true;
    case GenerateStep::Mode::Skip:
      return false;
    case GenerateStep::Mode::Rise:
      return !gateLatch.isRise();
    case GenerateStep::Mode::Fall:
      return !gateLatch.isFall();
    case GenerateStep::Mode::Edge:
      return !gateLatch.isEdge();
    case GenerateStep::Mode::High:
      return !gateLatch.isHigh();
    case GenerateStep::Mode::Low:
      return !gateLatch.isLow();
    }
  }

  GenerateStep::GenerateStep(StepControls &controls, int stepIndex) : controls{controls}, stepIndex{stepIndex} {}

  auto GenerateStep::isAvailable() const -> bool { return mode() != Mode::Skip; }

  auto GenerateStep::process(Latch const &gateLatch, float sampleTime) -> State {
    auto const isGenerating = isActive(mode(), gateLatch);
    if (isGenerating) {
      auto const state = generate(sampleTime);
      controls.setGenerating(stepIndex, state == State::Active);
      return state;
    }
    ramp.reset();
    controls.setGenerating(stepIndex, false);
    return State::Inactive;
  }

  auto GenerateStep::generate(float sampleTime) -> State {
    auto const phaseDelta = sampleTime / controls.duration(stepIndex);
    ramp.advance(phaseDelta);

    controls.setOutput(ramp.phase() * 8.F);

    //    controls.setOutput(dhe::scale(taper(ramp.phase()), 0.F, controls.level(stepIndex)));

    if (ramp.state() == OneShotPhaseAccumulator::State::Complete) {
      controls.setGenerating(stepIndex, false);
      ramp.reset();
      return State::Inactive;
    }
    return State::Active;
  }

  auto GenerateStep::taper(float phase) const -> float {
    return taper::variableJTaper.apply(phase, controls.curvature(stepIndex));
  }
} // namespace curve_sequencer
} // namespace dhe

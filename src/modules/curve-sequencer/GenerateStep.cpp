#include "modules/curve-sequencer/GenerateStep.h"

#include "modules/curve-sequencer/StepControls.h"

namespace dhe {
namespace curve_sequencer {
  static auto continuesOnRise(Step::Mode mode) -> bool {
    auto const ends = mode == Step::Mode::Rise || mode == Step::Mode::Edge || mode == Step::Mode::High;
    return !ends;
  }

  static auto continuesOnFall(Step::Mode mode) -> bool {
    auto const ends = mode == Step::Mode::Fall || mode == Step::Mode::Edge || mode == Step::Mode::Low;
    return !ends;
  }

  static auto continuesOnHigh(Step::Mode mode) -> bool { return mode != Step::Mode::High; }

  static auto continuesOnLow(Step::Mode mode) -> bool { return mode != Step::Mode::Low; }

  GenerateStep::GenerateStep(StepControls &controls, int stepIndex) : controls{controls}, stepIndex{stepIndex} {}

  auto GenerateStep::isAvailable() const -> bool { return mode() != Mode::Skip; }

  auto GenerateStep::process(Latch const &gateLatch, float sampleTime) -> State {
    auto const mode = this->mode();
    if (gateLatch.isRise()) {
      return process(sampleTime, continuesOnRise(mode));
    }
    if (gateLatch.isFall()) {
      return process(sampleTime, continuesOnFall(mode));
    }
    if (gateLatch.isHigh()) {
      return process(sampleTime, continuesOnHigh(mode));
    }
    return process(sampleTime, continuesOnLow(mode));
  }

  auto GenerateStep::process(float sampleTime, bool isGenerating) -> State {
    controls.setGenerating(stepIndex, isGenerating);
    if (!isGenerating) {
      return State::Inactive;
    }
    return State::Active;
  }
} // namespace curve_sequencer
} // namespace dhe

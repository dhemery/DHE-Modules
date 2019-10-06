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

  auto GenerateStep::process(Latch const &gateLatch, float /* ignored */) -> State {
    auto const mode = this->mode();
    if (gateLatch.isRise()) {
      return process(continuesOnRise(mode));
    }
    if (gateLatch.isFall()) {
      return process(continuesOnFall(mode));
    }
    if (gateLatch.isHigh()) {
      return process(continuesOnHigh(mode));
    }
    return process(continuesOnLow(mode));
  }

  auto GenerateStep::process(bool isGenerating) -> State {
    controls.setGenerating(stepIndex, isGenerating);
    return isGenerating ? State::Running : State::Finished;
  }
} // namespace curve_sequencer
} // namespace dhe

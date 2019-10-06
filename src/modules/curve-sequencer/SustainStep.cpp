#include "modules/curve-sequencer/SustainStep.h"

#include <modules/components/Latch.h>

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

  SustainStep::SustainStep(StepControls &controls, int stepIndex) : controls{controls}, stepIndex{stepIndex} {}

  auto SustainStep::isAvailable() const -> bool { return mode() != Mode::Skip; }

  auto SustainStep::process(Latch const &gateLatch, float /* ignored */) -> State {
    Mode mode = this->mode();
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

  auto SustainStep::process(bool isSustaining) -> Step::State {
    controls.setSustaining(stepIndex, isSustaining);
    return isSustaining ? State::Running : State::Finished;
  }

} // namespace curve_sequencer
} // namespace dhe

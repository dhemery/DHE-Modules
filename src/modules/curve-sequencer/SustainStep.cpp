#include "modules/curve-sequencer/SustainStep.h"

#include <modules/components/Latch.h>

namespace dhe {
namespace curve_sequencer {
  static auto isActive(SustainStep::Mode mode, Latch const &gateLatch) -> bool {
    switch (mode) {
    case SustainStep::Mode::Skip:
      return false;
    case SustainStep::Mode::Rise:
      return !gateLatch.isRise();
    case SustainStep::Mode::Fall:
      return !gateLatch.isFall();
    case SustainStep::Mode::Edge:
      return !gateLatch.isEdge();
    case SustainStep::Mode::High:
      return !gateLatch.isHigh();
    case SustainStep::Mode::Low:
      return !gateLatch.isLow();
    }
  }

  SustainStep::SustainStep(StepControls &controls, int stepIndex) : controls{controls}, stepIndex{stepIndex} {}

  auto SustainStep::isAvailable() const -> bool { return mode() != Mode::Skip; }

  auto SustainStep::process(Latch const &gateLatch, float /* ignored */) -> State {
    auto const isSustaining = isActive(mode(), gateLatch);
    controls.setSustaining(stepIndex, isSustaining);
    return isSustaining ? State::Active : State::Inactive;
  }
} // namespace curve_sequencer
} // namespace dhe

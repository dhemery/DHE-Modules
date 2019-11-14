#pragma once

#include "SequenceMode.h"
#include "StageMode.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  template <typename Controls> class SustainStage {
  public:
    SustainStage(Controls &controls) : controls{controls} {}

    auto isAvailable(int candidateStep, Latch const &gateLatch) const -> bool {
      if (!controls.isEnabled(candidateStep)) {
        return false;
      }
      auto const mode = controls.sustainMode(candidateStep);
      return isActive(mode, gateLatch);
    }

    void enter(int entryStep) {
      step = entryStep;
      showActive(true);
    }

    auto execute(dhe::Latch const &gateLatch) const -> SequenceMode {
      if (isActive(sustainMode(), gateLatch)) {
        return SequenceMode::Sustaining;
      }
      showActive(false);
      return SequenceMode::Advancing;
    };

    void exit() { showActive(false); }

  private:
    auto sustainMode() const -> StageMode { return controls.sustainMode(step); }

    void showActive(bool state) const { controls.showSustaining(step, state); }

    int step{0};
    Controls &controls;
  };
} // namespace curve_sequencer
} // namespace dhe

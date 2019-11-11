#pragma once

#include "SequenceMode.h"
#include "SequencerState.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {

  template <typename Controls> class Idle {
  public:
    Idle(Controls &controls) : controls{controls} {}

    auto execute(dhe::Latch const &gateLatch) const -> SequencerState {
      if (gateLatch.isRise()) {
        return {SequenceMode::Advancing, controls.selectionStart()};
      }
      return {SequenceMode::Idle, 0};
    };

  private:
    Controls &controls;
  };
}; // namespace curve_sequencer
} // namespace dhe
